/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

#include "HardwareSerial.h"
#include "HardwareSerial_private.h"

#include <AppHardwareApi.h>

// this next line disables the entire HardwareSerial.cpp, 
// this is so I can support Attiny series and any other chip without a uart
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3)

// SerialEvent functions are weak, so when the user doesn't define them,
// the linker just sets their address to 0 (which is checked below).
// The Serialx_available is just a wrapper around Serialx.available(),
// but we can refer to it weakly so we don't pull in the entire
// HardwareSerial instance if the user doesn't also refer to it.
#if defined(HAVE_HWSERIAL0)
  void serialEvent() __attribute__((weak));
  bool Serial0_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL1)
  void serialEvent1() __attribute__((weak));
  bool Serial1_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL2)
  void serialEvent2() __attribute__((weak));
  bool Serial2_available() __attribute__((weak));
#endif

#if defined(HAVE_HWSERIAL3)
  void serialEvent3() __attribute__((weak));
  bool Serial3_available() __attribute__((weak));
#endif

void serialEventRun(void)
{
#if defined(HAVE_HWSERIAL0)
  if (Serial0_available && serialEvent && Serial0_available()) serialEvent();
#endif
#if defined(HAVE_HWSERIAL1)
  if (Serial1_available && serialEvent1 && Serial1_available()) serialEvent1();
#endif
#if defined(HAVE_HWSERIAL2)
  if (Serial2_available && serialEvent2 && Serial2_available()) serialEvent2();
#endif
#if defined(HAVE_HWSERIAL3)
  if (Serial3_available && serialEvent3 && Serial3_available()) serialEvent3();
#endif
}

// Actual interrupt handlers //////////////////////////////////////////////////////////////

void HardwareSerial::_tx_udr_empty_irq(void)
{
}
extern "C" {
extern void vUTIL_UartText(char *pcString);
}
// Public Methods //////////////////////////////////////////////////////////////
char buf[32];
void HardwareSerial::begin(unsigned long baud, byte config)
{
	//UART
	vAHI_UartEnable(E_AHI_UART_0);

	vAHI_UartReset(E_AHI_UART_0, TRUE, TRUE);
	vAHI_UartReset(E_AHI_UART_0, FALSE, FALSE);

	int c_baud;
	switch(baud)
	{
	case 4800:   c_baud = E_AHI_UART_RATE_4800;   break;
	case 9600:   c_baud = E_AHI_UART_RATE_9600;   break;
	case 19200:  c_baud = E_AHI_UART_RATE_19200;  break;
	case 38400:  c_baud = E_AHI_UART_RATE_38400;  break;
	case 76800:  c_baud = E_AHI_UART_RATE_76800;  break;
	case 115200: c_baud = E_AHI_UART_RATE_115200; break;
	default:     c_baud = -1;                     break;
	}

	uint32_t wordlen;
	switch(config & 0x6)
	{
	case 0x0: wordlen = E_AHI_UART_WORD_LEN_5;  break;
	case 0x2: wordlen = E_AHI_UART_WORD_LEN_6;  break;
	case 0x4: wordlen = E_AHI_UART_WORD_LEN_7;  break;
	case 0x6: wordlen = E_AHI_UART_WORD_LEN_8;  break;
	default:  wordlen = -1; break;
	}

	uint32_t stopbit =
		( (config & 0x08) ? E_AHI_UART_1_STOP_BIT : E_AHI_UART_1_STOP_BIT);
	uint32_t parity_enable =
		( (config & 0x10) ? E_AHI_UART_PARITY_ENABLE : E_AHI_UART_PARITY_DISABLE);
	uint32_t parity_type =
		( (config & 0x30) ? E_AHI_UART_EVEN_PARITY: E_AHI_UART_ODD_PARITY);

	vAHI_UartSetClockDivisor(E_AHI_UART_0, c_baud);
	vAHI_UartSetControl(E_AHI_UART_0, parity_type, parity_enable, wordlen, stopbit, false);

#if DEBUG
	sprintf(buf, "c_baud:%d\r\n", c_baud);
	vUTIL_UartText(buf);
	sprintf(buf, "wordlen:%d\r\n", wordlen);
	vUTIL_UartText(buf);
	sprintf(buf, "stopbit:%d\r\n", stopbit);
	vUTIL_UartText(buf);
	sprintf(buf, "parity_enable:%d\r\n", parity_enable);
	vUTIL_UartText(buf);
	sprintf(buf, "parity_type:%d\r\n", parity_type);
	vUTIL_UartText(buf);
#endif

}

void HardwareSerial::end()
{
	vAHI_UartDisable(E_AHI_UART_0);
}

int peek_buf = -1;

int HardwareSerial::available(void)
{
	int r = u16AHI_UartReadRxFifoLevel(E_AHI_UART_0);
	if(peek_buf != -1) return r++;
	sprintf(buf, "available:%d\r\n", r);
	vUTIL_UartText(buf);
	return r;
}

int HardwareSerial::peek(void)
{
	if(peek_buf != -1) return peek_buf;

	peek_buf = read();
	sprintf(buf, "peek:%d\r\n", peek_buf);
	vUTIL_UartText(buf);
	return peek_buf;
}

int HardwareSerial::read(void)
{
	sprintf(buf, "read:\r\n");
	vUTIL_UartText(buf);
	int ret;
	if(peek_buf != -1) {
		ret = peek_buf;
		peek_buf = -1;
		return ret;
	}

	if(!u8AHI_UartReadLineStatus(E_AHI_UART_0) & E_AHI_UART_LS_DR) {
		return -1;
	}
	int r = u16AHI_UartReadRxFifoLevel(E_AHI_UART_0);
	sprintf(buf, "available:%d\r\n", r);
	vUTIL_UartText(buf);

	ret = u8AHI_UartReadData(E_AHI_UART_0);

	return ret;
}

int HardwareSerial::availableForWrite(void)
{
	return 0;
}

void HardwareSerial::flush()
{
	while((u8AHI_UartReadLineStatus(E_AHI_UART_0) & E_AHI_UART_LS_THRE) == 0);
}

size_t HardwareSerial::write(uint8_t c)
{
	sprintf(buf, "write:%d\r\n", c);
	vUTIL_UartText(buf);
	while((u8AHI_UartReadLineStatus(E_AHI_UART_0) & E_AHI_UART_LS_THRE) == 0);
	vAHI_UartWriteData(E_AHI_UART_0, c);

	return 1;
}


#endif // whole file
