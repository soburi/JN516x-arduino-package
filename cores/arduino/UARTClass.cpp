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
	: port(portno), initialized(false)
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
	vAHI_UartSetRTSCTS(port, false);
	vAHI_UartEnable(port);

	vAHI_UartReset(port, TRUE, TRUE);
	vAHI_UartReset(port, FALSE, FALSE);

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

	vAHI_UartSetClockDivisor(port, c_baud);
	vAHI_UartSetControl(port, parity_type, parity_enable, wordlen, stopbit, false);
	//vAHI_UartSetClockDivisor(port, E_AHI_UART_RATE_9600);
	//vAHI_UartSetControl(port, E_AHI_UART_EVEN_PARITY, false, E_AHI_UART_WORD_LEN_8, E_AHI_UART_1_STOP_BIT, false);

	DBG_PRINTF("c_baud:");
	DBG_PRINTF("%d\r\n", c_baud);
	DBG_PRINTF("\r\n");
	DBG_PRINTF("wordlen:");
	DBG_PRINTF("%d\r\n", wordlen);
	DBG_PRINTF("\r\n");
	DBG_PRINTF("stopbit:");
	DBG_PRINTF("%d\r\n", stopbit);
	DBG_PRINTF("\r\n");
	DBG_PRINTF("parity_enable:");
	DBG_PRINTF("%d\r\n", parity_enable);
	DBG_PRINTF("\r\n");
	DBG_PRINTF("parity_type:");
	DBG_PRINTF("%d\r\n", parity_type);
	DBG_PRINTF("\r\n");

	initialized = true;
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
	if(!initialized) return 0;

	DBG_PRINTF("available:");
	int r = u16AHI_UartReadRxFifoLevel(port);
	if(peek_buf != -1) return r++;
	DBG_PRINTF("%d\r\n", r);
	DBG_PRINTF("\r\n");
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
	DBG_PRINTF("peek:");
	DBG_PRINTF("%d\r\n", peek_buf);
	DBG_PRINTF("\r\n");
	return peek_buf;
}

int UARTClass::read( void )
{
	if(!initialized) return -1;

	DBG_PRINTF("read:\r\n");
	int ret;
	if(peek_buf != -1) {
		ret = peek_buf;
		peek_buf = -1;
		return ret;
	}

	if(!u8AHI_UartReadLineStatus(port) & E_AHI_UART_LS_DR) {
		return -1;
	}
	DBG_PRINTF("available:");
	DBG_PRINTF("%d\r\n", u16AHI_UartReadRxFifoLevel(port) );
	DBG_PRINTF("\r\n");
	ret = u8AHI_UartReadData(port);

	return ret;
}

void UARTClass::flush( void )
{
	if(!initialized) return;

	while((u8AHI_UartReadLineStatus(port) & E_AHI_UART_LS_THRE) == 0);
}

size_t UARTClass::write( const uint8_t uc_data )
{
	if(!initialized) return 0;

	while((u8AHI_UartReadLineStatus(port) & E_AHI_UART_LS_THRE) == 0);
	vAHI_UartWriteData(port, uc_data);
	return 1;
}

size_t UARTClass::write(const uint8_t *buffer, size_t size)
{
	if(!initialized) return 0;

	DBG_PRINTF("UARTClass::write(array)\r\n");
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

