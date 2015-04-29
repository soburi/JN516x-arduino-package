/*
  Copyright (c) 2011 Arduino.  All right reserved.

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "UARTClass.h"

#include "Arduino.h"
#include <AppHardwareApi.h>
//#define DEBUGPRINT_DISABLE(x)

// Constructors ////////////////////////////////////////////////////////////////

UARTClass::UARTClass(int portno)
	: port(portno)
{
}

/*
UARTClass::UARTClass( Uart *pUart, IRQn_Type dwIrq, uint32_t dwId, RingBuffer *pRx_buffer, RingBuffer *pTx_buffer )
{
  _rx_buffer = pRx_buffer;
  _tx_buffer = pTx_buffer;

  _pUart=pUart;
  _dwIrq=dwIrq;
  _dwId=dwId;
}
*/

// Public Methods //////////////////////////////////////////////////////////////

void UARTClass::begin(const uint32_t dwBaudRate)
{
  begin(dwBaudRate, Mode_8N1);
}

void UARTClass::begin(const uint32_t dwBaudRate, const UARTModes config)
{
  init(dwBaudRate, config);
}

void UARTClass::init(const uint32_t dwBaudRate, const uint32_t config)
{
	//UART
	vAHI_UartSetRTSCTS(E_AHI_UART_0, false);
	vAHI_UartEnable(E_AHI_UART_0);

	vAHI_UartReset(E_AHI_UART_0, TRUE, TRUE);
	vAHI_UartReset(E_AHI_UART_0, FALSE, FALSE);

	int c_baud;
	switch(dwBaudRate)
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
	switch( (config>>6) & 0x3)
	{
	case 0x0: wordlen = E_AHI_UART_WORD_LEN_5;  break;
	case 0x1: wordlen = E_AHI_UART_WORD_LEN_6;  break;
	case 0x2: wordlen = E_AHI_UART_WORD_LEN_7;  break;
	case 0x3: wordlen = E_AHI_UART_WORD_LEN_8;  break;
	default:  wordlen = -1; break;
	}

	uint32_t stopbit =
		( ((config>>12) & 0x01) ? E_AHI_UART_1_STOP_BIT : E_AHI_UART_1_STOP_BIT);
	uint32_t parity_enable =
		( ((config>>11) & 0x01) ? E_AHI_UART_PARITY_DISABLE : E_AHI_UART_PARITY_ENABLE);
	uint32_t parity_type =
		( ((config>>9) & 0x01) ? E_AHI_UART_EVEN_PARITY: E_AHI_UART_ODD_PARITY);

	vAHI_UartSetClockDivisor(E_AHI_UART_0, c_baud);
	vAHI_UartSetControl(E_AHI_UART_0, parity_type, parity_enable, wordlen, stopbit, false);
	//vAHI_UartSetClockDivisor(E_AHI_UART_0, E_AHI_UART_RATE_9600);
	//vAHI_UartSetControl(E_AHI_UART_0, E_AHI_UART_EVEN_PARITY, false, E_AHI_UART_WORD_LEN_8, E_AHI_UART_1_STOP_BIT, false);

	DEBUG_STR("c_baud:");
	DEBUG_DEC(c_baud);
	DEBUG_STR("\r\n");
	DEBUG_STR("wordlen:");
	DEBUG_DEC(wordlen);
	DEBUG_STR("\r\n");
	DEBUG_STR("stopbit:");
	DEBUG_DEC(stopbit);
	DEBUG_STR("\r\n");
	DEBUG_STR("parity_enable:");
	DEBUG_DEC(parity_enable);
	DEBUG_STR("\r\n");
	DEBUG_STR("parity_type:");
	DEBUG_DEC(parity_type);
	DEBUG_STR("\r\n");
}

void UARTClass::end( void )
{
	vAHI_UartDisable(port);
}
#if 0

void UARTClass::setInterruptPriority(uint32_t priority)
{
  NVIC_SetPriority(_dwIrq, priority & 0x0F);
}

uint32_t UARTClass::getInterruptPriority()
{
  return NVIC_GetPriority(_dwIrq);
}

#endif
int UARTClass::available( void )
{
	int r = u16AHI_UartReadRxFifoLevel(port);
	if(peek_buf != -1) return r++;
	DEBUG_STR("available:");
	DEBUG_DEC(r);
	DEBUG_STR("\r\n");
	return r;
}

int UARTClass::availableForWrite(void)
{
	//TODO
	return 0;
}

int UARTClass::peek( void )
{
	if(peek_buf != -1) return peek_buf;

	peek_buf = read();
	DEBUG_STR("peek:");
	DEBUG_DEC(peek_buf);
	DEBUG_STR("\r\n");
	return peek_buf;
}

int UARTClass::read( void )
{
	DEBUG_STR("read:\r\n");
	int ret;
	if(peek_buf != -1) {
		ret = peek_buf;
		peek_buf = -1;
		return ret;
	}

	if(!u8AHI_UartReadLineStatus(port) & E_AHI_UART_LS_DR) {
		return -1;
	}
	DEBUG_STR("available:");
	DEBUG_DEC(u16AHI_UartReadRxFifoLevel(port) );
	DEBUG_STR("\r\n");
	ret = u8AHI_UartReadData(port);

	return ret;
}

void UARTClass::flush( void )
{
	while((u8AHI_UartReadLineStatus(port) & E_AHI_UART_LS_THRE) == 0);
}

size_t UARTClass::write( const uint8_t uc_data )
{
	while((u8AHI_UartReadLineStatus(port) & E_AHI_UART_LS_THRE) == 0);
	vAHI_UartWriteData(port, uc_data);
	return 1;
}

size_t UARTClass::write(const uint8_t *buffer, size_t size)
{
	DEBUG_STR("UARTClass::write(array)\r\n");
	while((u8AHI_UartReadLineStatus(port) & E_AHI_UART_LS_THRE) == 0);
	for(int i=0; i<size; i++) {
		vAHI_UartWriteData(port, buffer[i]);
	}
	return size;
}

void UARTClass::IrqHandler( void )
{
	//TODO
}

