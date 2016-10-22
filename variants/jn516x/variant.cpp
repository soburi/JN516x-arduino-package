/*
  Copyright (c) 2016 TOKITA Hiroshi.  All right reserved.

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

#define D_PLATFORM_UART(x) platform_uart ## x
#define D_PLATFORM_UART_F(x,y) platform_uart ## x ## _ ## y
#define D_SERIAL(x) Serial ## x
#define D_SERIALEVENT(x) serialEvent ## x

#define DECL_UART(x)\
static int UART ## x ## _RxHandler(uint8_t);\
extern "C" void D_PLATFORM_UART_F(x,init)(void*, uint32_t, uint8_t, uint8_t, uint8_t, uint8_t); \
extern "C" void D_PLATFORM_UART_F(x,set_input)(void*, int (*input)(uint8_t)); \
extern "C" void D_PLATFORM_UART_F(x,writeb)(void*, uint8_t); \
extern "C" uint8_t D_PLATFORM_UART_F(x,busy)(void*); \
extern "C" void D_PLATFORM_UART_F(x,deinit)(void*); \
struct uart_device D_PLATFORM_UART(x) = {\
	UART##x##_RxHandler, \
	D_PLATFORM_UART_F(x,init), \
	D_PLATFORM_UART_F(x,set_input), \
	D_PLATFORM_UART_F(x,writeb), \
	D_PLATFORM_UART_F(x,busy), \
	D_PLATFORM_UART_F(x,deinit), \
	NULL, \
	0 \
};\
\
static int UART ## x ## _RxHandler(uint8_t c) \
{ \
  D_PLATFORM_UART(x).received = c; \
  D_SERIAL(x).IrqHandler(); \
  D_PLATFORM_UART(x).received = 0; \
  return 0; \
} \
\
Uart D_SERIAL(x)(&D_PLATFORM_UART(x));\
void D_SERIALEVENT(x)() __attribute__((weak));\
void D_SERIALEVENT(x)() { }\

#ifdef HAS_UART_0
DECL_UART(0)
#endif
#ifdef HAS_UART_1
DECL_UART(1)
#endif
#ifdef HAS_UART_2
DECL_UART(2)
#endif
#ifdef HAS_UART_3
DECL_UART(3)
#endif

//alias
Uart& Serial = Serial0;
void serialEvent() __attribute__((weak));
void serialEvent() { serialEvent0(); }


void serialEventRun(void)
{
  if (Serial.available()) serialEvent();
  if (Serial1.available()) serialEvent1();
}

// ----------------------------------------------------------------------------

void platform_uart0_init(void* port, uint32_t baudrate, uint8_t parity, uint8_t stopbit, uint8_t wordlen, uint8_t flowctrl)
{
	int baud;
	switch(baudrate)
	{
	case 4800:   baud = UART_RATE_4800;   break;
	case 9600:   baud = UART_RATE_9600;   break;
	case 19200:  baud = UART_RATE_19200;  break;
	case 38400:  baud = UART_RATE_38400;  break;
	case 76800:  baud = UART_RATE_76800;  break;
	case 115200: baud = UART_RATE_115200; break;
	default:     baud = -1;               break;
	}
	
	(void)port; (void)parity; (void)stopbit; (void)wordlen; (void)flowctrl;
	uart0_init(baud);
}

void platform_uart0_set_input(void* port, int (*input)(uint8_t))
{
	(void)port;
	uart0_set_input(input);
}
void platform_uart0_writeb(void* port, uint8_t c)
{
	(void)port;
	uart0_writeb(c);
}
uint8_t platform_uart0_busy(void* port)
{
	(void)port;
	return uart0_active();
}
void platform_uart0_deinit(void* port)
{
	(void)port;
}


void platform_uart1_init(void* port, uint32_t baudrate, uint8_t parity, uint8_t stopbit, uint8_t wordlen, uint8_t flowctrl)
{
	(void)port; (void)parity; (void)stopbit; (void)wordlen; (void)flowctrl;
	uart1_init(baudrate);
}

void platform_uart1_set_input(void* port, int (*input)(uint8_t))
{
	(void)port;
	uart1_set_input(input);
}
void platform_uart1_writeb(void* port, uint8_t c)
{
	(void)port;
	uart1_writeb(c);
}
uint8_t platform_uart1_busy(void* port)
{
	(void)port;
	return uart1_active();
}
void platform_uart1_deinit(void* port)
{
	(void)port;
}
