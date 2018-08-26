/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

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

#include "Uart.h"
#include "Arduino.h"
#include "wiring_private.h"

Uart::Uart(struct uart_device *_s)
{
  uart=_s;
}

void Uart::begin(unsigned long baudrate)
{
	begin(baudrate, SERIAL_8N1);
}

void Uart::begin(unsigned long baudrate, uint16_t config)
{
	uint8_t parity  =  config & HARDSER_PARITY_MASK;
	uint8_t stopbit = (config & HARDSER_STOP_BIT_MASK) >> 4;
	uint8_t wordlen = ((config & HARDSER_DATA_MASK) >> 8) + 4;
	uint8_t flowctrl = 0;
	
	uart->init(uart->portinfo, baudrate, parity, stopbit, wordlen, flowctrl);
	uart->set_input(uart->portinfo, uart->input);
}

void Uart::end()
{
	flush();
	uart->deinit(uart->portinfo);
}

void Uart::flush()
{
	while (uart->busy(uart->portinfo) ) ;
}

void Uart::IrqHandler()
{
	rxBuffer.store_char(uart->received);
}

int Uart::available()
{
	return (uint32_t)(SERIAL_BUFFER_SIZE + rxBuffer._iHead - rxBuffer._iTail) % SERIAL_BUFFER_SIZE;
}

int Uart::availableForWrite()
{
	return uart->txbuffer_availables(uart->portinfo);
}

int Uart::peek()
{
	return rxBuffer.peek();
}

int Uart::read()
{
	return rxBuffer.read_char();
}

size_t Uart::write(const uint8_t data)
{
	uart->writeb(uart->portinfo, data);
	return 1;
}
