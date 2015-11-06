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

#include "platform.h"

/*
 * UART objects
 */
RingBuffer rx_buffer1;
RingBuffer tx_buffer1;

Uart uart0_funcs = {
	uart0_active,
	uart0_set_input,
	uart0_writeb,
	uart0_init,
	UART0_RxHandler
};

UARTClass Serial(&uart0_funcs, 0, E_AHI_UART_0, &rx_buffer1, &tx_buffer1);
void serialEvent() __attribute__((weak));
void serialEvent() { }

// IT handlers
int UART0_RxHandler(uint8_t c)
{
  return Serial.RxHandler(c);
}

// ----------------------------------------------------------------------------
/*
 * USART objects
 */
RingBuffer rx_buffer2;


RingBuffer tx_buffer2;


Uart uart1_funcs = {
	uart1_active,
	uart1_set_input,
	uart1_writeb,
	uart1_init,
	UART1_RxHandler
};

UARTClass Serial1(&uart1_funcs, 0, E_AHI_UART_1, &rx_buffer2, &tx_buffer2);
void serialEvent1() __attribute__((weak));
void serialEvent1() { }


// IT handlers
int UART1_RxHandler(uint8_t c)
{
  return Serial.RxHandler(c);
}

// ----------------------------------------------------------------------------

void serialEventRun(void)
{
  if (Serial.available()) serialEvent();
  if (Serial1.available()) serialEvent1();
}

// ----------------------------------------------------------------------------

bool warmBoot()
{
	return (u16AHI_PowerStatus() & POWER_STATUS_RAM_RETAINING) ? 1 : 0;
}

bool waked()
{
	return (u16AHI_PowerStatus() & POWER_STATUS_WAKED) ? 1 : 0;
}

uint32_t waketimer_calibration_value = 0;

uint32_t wakeTimerCalibrationValue()
{
	return waketimer_calibration_value;
}
void setWakeTimerCalibrationValue(uint32_t value)
{
	waketimer_calibration_value = value;
}
