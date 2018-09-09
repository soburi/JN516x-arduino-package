/*
  Copyright (c) 2015-2018 Tokita, Hiroshi  All right reserved.

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
extern "C" {
#include "i2c-driver.h"
#include "spi-driver.h"
#include "power-driver.h"
#include <uart0.h>
#include <uart1.h>
}

#define D_PLATFORM_UART(x) platform_uart ## x
#define D_PLATFORM_UART_F(x,y) platform_uart ## x ## _ ## y
#define D_SERIAL(x) Serial ## x
#define D_SERIALEVENT(x) serialEvent ## x

#define DECL_UART(x)\
static int UART ## x ## _RxHandler(uint8_t);\
extern "C" void D_PLATFORM_UART_F(x,init)(void*, uint32_t, uint8_t, uint8_t, uint8_t, uint8_t); \
extern "C" void D_PLATFORM_UART_F(x,set_input)(void*, int (*input)(uint8_t)); \
extern "C" void D_PLATFORM_UART_F(x,writeb)(void*, uint8_t); \
extern "C" int D_PLATFORM_UART_F(x,txbuffer_availables)(void*); \
extern "C" uint8_t D_PLATFORM_UART_F(x,busy)(void*); \
extern "C" void D_PLATFORM_UART_F(x,deinit)(void*); \
struct uart_device D_PLATFORM_UART(x) = {\
	UART##x##_RxHandler, \
	D_PLATFORM_UART_F(x,init), \
	D_PLATFORM_UART_F(x,set_input), \
	D_PLATFORM_UART_F(x,writeb), \
	D_PLATFORM_UART_F(x,busy), \
	D_PLATFORM_UART_F(x,txbuffer_availables), \
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
	
	(void)baud;
	(void)port; (void)parity; (void)stopbit; (void)wordlen; (void)flowctrl;
#ifndef DBG_ENABLE
	uart0_init(baud);
#endif
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
int platform_uart0_txbuffer_availables(void* port)
{
	(void)port;
	return UART_TX_BUFFER_SIZE - u16AHI_UartReadTxFifoLevel(E_AHI_UART_0);
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
int platform_uart1_txbuffer_availables(void* port)
{
	(void)port;
	return UART1_TX_BUFFER_SIZE - u16AHI_UartReadTxFifoLevel(E_AHI_UART_1);
}
void platform_uart1_deinit(void* port)
{
	(void)port;
}

struct i2c_device i2c0 = {
	jn516x_i2c_init,
	jn516x_i2c_master_enable,
	jn516x_i2c_slave_enable,
	jn516x_i2c_disable,
	jn516x_i2c_master_start,
	jn516x_i2c_master_read,
	jn516x_i2c_master_write,
	jn516x_i2c_master_stop,
	jn516x_i2c_slave_write,
	jn516x_i2c_rx_available,
	jn516x_i2c_rx_clear,
	jn516x_i2c_rx_read,
	jn516x_i2c_rx_peek,
	jn516x_i2c_tx_available,
	jn516x_i2c_rx_clear,
	jn516x_i2c_tx_put,
	jn516x_i2c_tx_full,
	jn516x_i2c_request_received,
	jn516x_i2c_data_received,
	&jn516x_i2c_info
};

int platform_spi_init(void* /*dev*/)
{
	spi_init();
	return 0;
}

int platform_spi_deinit(void* /*dev*/)
{
	spi_deinit();
	return 0;
}

int platform_spi_configure(void* /*dev*/, bool msbFirst, bool polarity, bool phase, uint32_t clock)
{
	spi_configure(0, !msbFirst, polarity, phase, (SystemCoreClock/clock)/2);
	return 0;
}

int platform_spi_start(void* /*dev*/)
{
	spi_start(0);
	return 0;
}

void platform_spi_stop(void* /*dev*/)
{
	spi_stop();
}

uint8_t platform_spi_transfer(void* /*dev*/, uint8_t data)
{
	spi_write(data);
	uint8_t ret = spi_read();
	return ret;
}

int platform_spi_mask_interrupt_on_transaction(void* /*dev*/, int /*interrupt*/)
{
	return 0;
}

struct spi_device spi0 = {
	platform_spi_init,
	platform_spi_configure,
	platform_spi_start,
	platform_spi_stop,
	platform_spi_transfer,
	platform_spi_mask_interrupt_on_transaction,
	platform_spi_deinit,
	NULL
};

struct power_device pow0 = {
	power_idle,
	power_sleep,
	power_deepsleep,
	power_wakeup_reason,
	NULL
};
