/*
  Copyright (c) 2016 Tokita, Hiroshi  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "variant.h"
#include "platform.h"
#include "wiring_private.h"

#include <cerrno>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <termios.h>
#include <pthread.h>
#include <sys/ioctl.h>


#ifndef CONSOLE_SERIAL_MAP
#define CONSOLE_SERIAL_MAP 0
#endif

struct receive_thread_params {
	int (*input)(uint8_t);
	int fd;
	pthread_t thread;
} __attribute__((aligned(4)));

static int NOP_UART_RxHandler(uint8_t c);
static void NOP_UART_init(void* x, uint32_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e);
static void NOP_UART_input(void* x, int (*input)(uint8_t));
static uint8_t NOP_UART_busy(void* x);
static void NOP_UART_deinit(void* x);

static int uart0_rxhandler(uint8_t c);
static void uart0_init(void* fd, uint32_t baudrate, uint8_t parity, uint8_t stopbit, uint8_t wordlen, uint8_t flowctrl);
static void uart0_set_input(void* fd, int (*input)(uint8_t));
static void uart0_writeb(void* fd, uint8_t ch);
static uint8_t uart0_busy(void* fd);
static void uart0_deinit(void* fd);

static int stdin_rxhandler(uint8_t c);
static void stdin_init(void* fd, uint32_t baudrate, uint8_t parity, uint8_t stopbit, uint8_t wordlen, uint8_t flowctrl);
static void stdin_set_input(void* fd, int (*input)(uint8_t));
static void stdout_writeb(void* x, uint8_t ch);
static void stderr_writeb(void* x, uint8_t ch);

static void* receive_thread(void* arg);

static int uart0_fd;
static struct uart_device platform_uart0 = {
	uart0_rxhandler,
	uart0_init,
	uart0_set_input,
	uart0_writeb,
	uart0_busy,
	uart0_deinit,
	&uart0_fd,
	0
};

void serialEvent() __attribute__((weak));
void serialEvent() { }

static int stdin_fd;
static struct uart_device platform_console = {
	stdin_rxhandler,
	stdin_init,
	stdin_set_input,
	stdout_writeb,
	NOP_UART_busy,
	NOP_UART_deinit,
	&stdin_fd,
	0
};

static struct uart_device platform_stderr = {
	NOP_UART_RxHandler,
	NOP_UART_init,
	NOP_UART_input,
	stderr_writeb,
	NOP_UART_busy,
	NOP_UART_deinit,
	NULL,
	0
};

static struct receive_thread_params uart0_receive_thread_params;
static struct receive_thread_params stdin_receive_thread_params;

#if CONSOLE_SERIAL_MAP == 0
Uart Serial(&platform_console);
Uart Serial1(&platform_uart0);
Uart& Console(Serial);
Uart& Uart0(Serial1);
#else
Uart Serial(&platform_uart0);
Uart Serial1(&platform_console);
Uart& Console(Serial1);
Uart& Uart0(Serial);
#endif

Uart Stderr(&platform_stderr);

static int NOP_UART_RxHandler(uint8_t c)
{
	(void)c;
	return 0;
}
static void NOP_UART_init(void* x, uint32_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e)
{
	(void)x; (void)a; (void)b; (void)c; (void)d; (void)e;
}
static void NOP_UART_input(void* x, int (*input)(uint8_t))
{
	(void)x; (void)input;
}
//static void NOP_UART_writeb(void* x, uint8_t ch)
//{
//	(void)x; (void)ch;
//}
static uint8_t NOP_UART_busy(void* x)
{
	(void)x;
	return 0;
}
static void NOP_UART_deinit(void* x)
{
	(void)x;
}

static int uart0_rxhandler(uint8_t c)
{
	LOG_DEBUG("uart0_rxhandler c=%d\n", c);
	platform_uart0.received = c;
	Uart0.IrqHandler();
	platform_uart0.received = 0;
	return 0;
}

static void uart0_init(void* pfd, uint32_t baudrate, uint8_t parity, uint8_t stopbit, uint8_t wordlen, uint8_t /*flowctrl*/)
{
	LOG_DEBUG("uart0_init b=%u p=%u s=%u w=%u\n", baudrate, parity, stopbit, wordlen);

	int fd;
	int ret;
	const char* device = NULL;
	device = "/dev/serial0";

	//LOG_DEBUG("open\n");
	fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	ABORT_ON_ERROR(fd<0, "%s open(%s) failure:%d %s", __FUNCTION__, device, errno, strerror(errno) );

	speed_t speed;
	switch (baudrate)
	{
	case     50:	speed =     B50; break;
	case     75:	speed =     B75; break;
	case    110:	speed =    B110; break;
	case    134:	speed =    B134; break;
	case    150:	speed =    B150; break;
	case    200:	speed =    B200; break;
	case    300:	speed =    B300; break;
	case    600:	speed =    B600; break;
	case   1200:	speed =   B1200; break;
	case   1800:	speed =   B1800; break;
	case   2400:	speed =   B2400; break;
	case   4800:	speed =   B4800; break;
	case   9600:	speed =   B9600; break;
	case  19200:	speed =  B19200; break;
	case  38400:	speed =  B38400; break;
	case  57600:	speed =  B57600; break;
	case 115200:	speed = B115200; break;
	case 230400:	speed = B230400; break;
	default:	speed =   B9600; break;
	}

	ret = fcntl(fd, F_SETFL, O_RDWR);
	ABORT_ON_ERROR(ret<0, "%s fcntl(%d, F_SETFL, O_RDWR) failure:%d %s", __FUNCTION__, fd, errno, strerror(errno) );

	struct termios options;
	ret = tcgetattr(fd, &options);
	ABORT_ON_ERROR(ret<0, "%s tcgetattr(%d, %p) failure:%d %s", __FUNCTION__, fd, &options, errno, strerror(errno) );

	ret = cfsetspeed(&options, speed);
	ABORT_ON_ERROR(ret<0, "%s cfsetspeed(%p, %d) failure:%d %s", __FUNCTION__, &options, speed, errno, strerror(errno) );

	cfmakeraw(&options);

	switch(parity) {
	case HARDSER_PARITY_EVEN:
		options.c_cflag |=  PARENB;
		options.c_cflag &= ~PARODD;
		break;
	case HARDSER_PARITY_ODD:
		options.c_cflag |=  PARENB;
		options.c_cflag |=  PARODD;
		break;
	case HARDSER_PARITY_NONE: default:
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~PARODD;
		break;
	}

	if(stopbit == 1) {
		options.c_cflag &= ~CSTOPB;
	}
	else {
		options.c_cflag |=  CSTOPB;
	}

	switch(wordlen) {
	case 5:
		options.c_cflag |= CSIZE;
		options.c_cflag |= CS5;
		break;
	case 6:
		options.c_cflag |= CSIZE;
		options.c_cflag |= CS6;
		break;
	case 7:
		options.c_cflag |= CSIZE;
		options.c_cflag |= CS7;
		break;
	case 8: default:
		options.c_cflag |= CSIZE;
		options.c_cflag |= CS8;
		break;
	}

	//if(flowctrl) {
	//}
	//

	options.c_cc [VMIN]  = 0;
	options.c_cc [VTIME] = UINT8_MAX;

	ret = tcsetattr(fd, TCSANOW | TCSAFLUSH, &options) ;
	ABORT_ON_ERROR(ret<0, "%s tcsetattr(%d, TCSANOW | TCSAFLUSH, %p) failure:%d %s", __FUNCTION__, fd, &options, errno, strerror(errno) );

	*(reinterpret_cast<int*>(pfd)) = fd ;
}
static void uart0_set_input(void* pfd, int (*input)(uint8_t))
{
	int fd = *reinterpret_cast<int*>(pfd);
	LOG_DEBUG("%s fd=%d, input=%p\n", __FUNCTION__, fd, input);

	uart0_receive_thread_params.fd = fd;
	uart0_receive_thread_params.input = input;
	LOG_DEBUG("%s %p input \n", __FUNCTION__, input);
	LOG_DEBUG("%s %p uart0_rxhandler \n", __FUNCTION__, uart0_rxhandler);
	LOG_DEBUG("%s %p uart0_receive_thread_params\n", __FUNCTION__, &uart0_receive_thread_params);
	pthread_create(&uart0_receive_thread_params.thread, NULL, receive_thread, &uart0_receive_thread_params);
}
static void uart0_writeb(void* pfd, uint8_t ch)
{
	int fd = *reinterpret_cast<int*>(pfd);
	LOG_DEBUG("%s fd=%d, ch=%d\n", __FUNCTION__, fd, ch);

	int ret = write(fd, &ch, 1) ;
	WARN_ON_ERROR(ret<0, "%s write(%d, %c, 1) failure:%d %s", __FUNCTION__, fd, ch, errno, strerror(errno) );
}

static uint8_t uart0_busy(void* pfd)
{
	int fd = *reinterpret_cast<int*>(pfd);
	LOG_DEBUG("%s fd=%d\n", __FUNCTION__, fd);

	int countq;
	int ret = ioctl(fd, TIOCOUTQ, &countq);
	WARN_ON_ERROR(ret<0, "%s ioctl(%d, TIOCOUTQ, %p) failure:%d %s", __FUNCTION__, fd, &countq, errno, strerror(errno) );
	return countq;
}
static void uart0_deinit(void* pfd)
{
	int fd = *reinterpret_cast<int*>(pfd);
	LOG_DEBUG("%s fd=%d\n", __FUNCTION__, fd);

	close(fd);
}

static int stdin_rxhandler(uint8_t c)
{
	platform_console.received = c;
	Console.IrqHandler();
	platform_console.received = 0;
	return 0;
}
static void stdin_init(void* pfd, uint32_t /*baudrate*/, uint8_t /*parity*/, uint8_t /*stopbit*/, uint8_t /*wordlen*/, uint8_t /*flowctrl*/)
{
	struct termios settings;

	int ret;
	int fd = fileno(stdin);
	
	ret = tcgetattr(fd, &settings );
	ABORT_ON_ERROR(ret<0, "%s tcgetattr(%d, %p) failure:%d %s", __FUNCTION__, fd, &settings, errno, strerror(errno) );
	
	settings.c_lflag &= ~( ECHO | ICANON );
	ret = tcsetattr(fd, TCSANOW, &settings );
	ABORT_ON_ERROR(ret<0, "%s tcsetattr(%d, TCSANOW, %p) failure:%d %s", __FUNCTION__, fd, &settings, errno, strerror(errno) );
	ret = fcntl(fd, F_SETFL, O_NONBLOCK );
	ABORT_ON_ERROR(ret<0, "%s fcntl(%d, F_SETFL, O_RDWR) failure:%d %s", __FUNCTION__, fd, errno, strerror(errno) );

	*(reinterpret_cast<int*>(pfd)) = fd;
}

static void stdin_set_input(void* pfd, int (*input)(uint8_t))
{
	stdin_receive_thread_params.fd = *(reinterpret_cast<int*>(pfd));
	stdin_receive_thread_params.input = input;
	LOG_DEBUG("stdin_set_input %p\n", &stdin_receive_thread_params);
	pthread_create(&stdin_receive_thread_params.thread, NULL, receive_thread, &stdin_receive_thread_params);
}

static void stdout_writeb(void* x, uint8_t ch)
{
	(void)x;
	int ret = putc(ch, stdout);
	WARN_ON_ERROR(ret<0, "%s putc(%c, stdout) failure:%d %s", __FUNCTION__, ch, errno, strerror(errno) );
}

static void stderr_writeb(void* x, uint8_t ch)
{
	(void)x;
	int ret = putc(ch, stderr);
	WARN_ON_ERROR(ret<0, "%s putc(%c, stderr) failure:%d %s", __FUNCTION__, ch, errno, strerror(errno) );
}

static void* receive_thread(void* arg)
{
	struct receive_thread_params *params = reinterpret_cast<struct receive_thread_params*>(arg);

	struct pollfd poll_instr;
	poll_instr.fd = params->fd;
	poll_instr.events = POLLIN;
	poll_instr.revents = 0;

	char* readbuf = new char[512];

	while(true) {
		int ret = poll(&poll_instr, 1, -1);
		LOG_DEBUG("poll() ret=%d, revents=%d, %s\n", ret, poll_instr.revents, strerror(errno) );
		if(ret > 0 && poll_instr.revents & POLLIN) {
			ret = read(params->fd, &readbuf, 512);
			LOG_DEBUG("read ret=%d %s\n", ret, strerror(errno) );
			for(int i=0; i<ret; i++) {
				params->input(readbuf[i]);
			}
		}
	}
}

// ----------------------------------------------------------------------------

void serialEventRun(void)
{
  if (Serial.available()) serialEvent();
}

// ----------------------------------------------------------------------------
