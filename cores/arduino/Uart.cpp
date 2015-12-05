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

Uart::Uart(SERCOM *_s, uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX)
{
  sercom=_s;
  (void)_pinRX;
  (void)_pinTX;
  (void)_padRX;
  (void)_padTX;
}

void Uart::begin(unsigned long baudrate)
{
  begin(baudrate, (uint8_t)SERIAL_8N1);
}

void Uart::begin(unsigned long baudrate, uint8_t config)
{
	uint8_t parity  =  config && HARDSER_PARITY_MASK;
	uint8_t stopbit = (config && HARDSER_STOP_BIT_MASK) >> 8;
	uint8_t wordlen = (config && HARDSER_DATA_MASK)     >> 16;
	uint8_t flowctrl = 0;
	
	sercom->set_input(sercom->portinfo, sercom->input);
	sercom->init(sercom->portinfo, baudrate, parity, stopbit, wordlen, flowctrl);
}

void Uart::end()
{
	flush();
	sercom->deinit(sercom->portinfo);
}

void Uart::flush()
{
	while (sercom->busy(sercom->portinfo) ) ;
}

void Uart::IrqHandler()
{
	rxBuffer.store_char(sercom->received);
}

int Uart::available()
{
	return (uint32_t)(SERIAL_BUFFER_SIZE + rxBuffer._iHead - rxBuffer._iTail) % SERIAL_BUFFER_SIZE;
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
	sercom->writeb(sercom->portinfo, data);
	return 1;
}
