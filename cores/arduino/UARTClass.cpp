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

extern "C" {
#include <contiki.h>
#include <dev/uart-driver.h>
}

// Constructors ////////////////////////////////////////////////////////////////

UARTClass::UARTClass( Uart *pUart, IRQn_Type dwIrq, uint32_t dwId, RingBuffer *pRx_buffer, RingBuffer *pTx_buffer )
{
  _rx_buffer = pRx_buffer;
  _tx_buffer = pTx_buffer;

  _pUart=pUart;
  //_dwIrq=dwIrq;
  _dwId=dwId;
}

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
	int c_baud;
	switch(dwBaudRate)
	{
	case 4800:   c_baud = UART_RATE_4800;   break;
	case 9600:   c_baud = UART_RATE_9600;   break;
	case 19200:  c_baud = UART_RATE_19200;  break;
	case 38400:  c_baud = UART_RATE_38400;  break;
	case 76800:  c_baud = UART_RATE_76800;  break;
	case 115200: c_baud = UART_RATE_115200; break;
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

	//vAHI_UartSetControl(_dwId, parity_type, parity_enable, wordlen, stopbit, false);
	_pUart->set_input(_pUart->input);
	_pUart->init(c_baud);
}

void UARTClass::end( void )
{
  flush();
  vAHI_UartDisable(_dwId);
}

int UARTClass::available( void )
{
  return (uint32_t)(SERIAL_BUFFER_SIZE + _rx_buffer->_iHead - _rx_buffer->_iTail) % SERIAL_BUFFER_SIZE;
}

int UARTClass::availableForWrite(void)
{
  return UART_TX_BUFFER_SIZE - u16AHI_UartReadTxFifoLevel(_dwId);
}

int UARTClass::peek( void )
{
  if ( _rx_buffer->_iHead == _rx_buffer->_iTail )
    return -1;

  return _rx_buffer->_aucBuffer[_rx_buffer->_iTail];
}

int UARTClass::read( void )
{
  // if the head isn't ahead of the tail, we don't have any characters
  if ( _rx_buffer->_iHead == _rx_buffer->_iTail )
    return -1;

  uint8_t uc = _rx_buffer->_aucBuffer[_rx_buffer->_iTail];
  _rx_buffer->_iTail = (unsigned int)(_rx_buffer->_iTail + 1) % SERIAL_BUFFER_SIZE;
  return uc;
}

void UARTClass::flush( void )
{
  while (_pUart->active() ) ;
}

size_t UARTClass::write( const uint8_t uc_data )
{
  _pUart->writeb(uc_data);
  return 1;
}

int UARTClass::RxHandler(const uint8_t c)
{
  _rx_buffer->store_char(c);
  return 0;
}

