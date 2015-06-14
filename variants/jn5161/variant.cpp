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

#include "variant.h"

#include <AppHardwareApi.h>

/*
 * UART objects
 */
RingBuffer rx_buffer1;
RingBuffer tx_buffer1;

UARTClass Serial(E_AHI_UART_0, &rx_buffer1, &tx_buffer1);
void serialEvent() __attribute__((weak));
void serialEvent() { }

// IT handlers
void UART0_Handler(uint32_t u32DeviceId, uint32_t u32ItemBitmap)
{
  Serial.IrqHandler(u32ItemBitmap);
}

// ----------------------------------------------------------------------------
/*
 * USART objects
 */
RingBuffer rx_buffer2;


RingBuffer tx_buffer2;



UARTClass Serial1(E_AHI_UART_1, &rx_buffer2, &tx_buffer2);
void serialEvent1() __attribute__((weak));
void serialEvent1() { }

// IT handlers
void UART1_Handler(uint32_t u32DeviceId, uint32_t u32ItemBitmap)
{
  Serial1.IrqHandler(u32ItemBitmap);
}

// ----------------------------------------------------------------------------

void serialEventRun(void)
{
  if (Serial.available()) serialEvent();
  if (Serial1.available()) serialEvent1();
}

// ----------------------------------------------------------------------------
