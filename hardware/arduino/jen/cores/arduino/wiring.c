/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id$
*/

#include "wiring_private.h"

#include <AppHardwareApi.h>
#define MICROSECONDS_PER_TICKTIMER_OVERFLOW (clockCyclesToMicroseconds(0x0FFFFFFF))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TICKTIMER_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TICKTIMER_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long ticktimer_overflow_count = 0;
volatile unsigned long ticktimer_millis = 0;

#define TICK_TIMER_MAX (0x0fffffff)

static void ticktimer_callback(uint32 u32Device, uint32 u32ItemBitmap)
{
	ticktimer_overflow_count++;
}

unsigned long millis()
{
	unsigned long m;
	m = MILLIS_INC * ticktimer_overflow_count +
		(clockCyclesToMicroseconds(u32AHI_TickTimerRead())/1000);
	DEBUG_STR("millis: ");
	DEBUG_DEC(m);
	DEBUG_STR("\r\n");
	return m;
}

unsigned long micros() {
	unsigned long m;

	m = MICROSECONDS_PER_TICKTIMER_OVERFLOW * ticktimer_overflow_count +
		clockCyclesToMicroseconds(u32AHI_TickTimerRead());
	return m;
}

void delay(unsigned long ms)
{
	uint32_t start = (uint32_t)micros();

	while (ms > 0) {
		yield();
		if (((uint32_t)micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}

	DEBUG_STR("delay: ");
	DEBUG_DEC(micros());
	DEBUG_STR("\r\n");
}

/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */
void delayMicroseconds(unsigned int us)
{
}

void init()
{
	u32AHI_Init();

#ifdef USE_DEBUGPRINT
	//UART
	vAHI_UartEnable(E_AHI_UART_0);
	vAHI_UartReset(E_AHI_UART_0, TRUE, TRUE);
	vAHI_UartReset(E_AHI_UART_0, FALSE, FALSE);
	vAHI_UartSetClockDivisor(E_AHI_UART_0, E_AHI_UART_RATE_9600);
	vAHI_UartSetControl(E_AHI_UART_0, E_AHI_UART_EVEN_PARITY, E_AHI_UART_PARITY_DISABLE, E_AHI_UART_WORD_LEN_8, E_AHI_UART_1_STOP_BIT, false);
#else
	vAHI_UartDisable(E_AHI_UART_0);
	vAHI_UartDisable(E_AHI_UART_1);
#endif
	//Tick Timer
	vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_DISABLE);
	vAHI_TickTimerWrite(0);
	vAHI_TickTimerInterval(TICK_TIMER_MAX);
	vAHI_TickTimerIntEnable(true);

	vAHI_TickTimerRegisterCallback(ticktimer_callback);
	vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_RESTART);

	//PWM Timer
	vAHI_TimerSetLocation(E_AHI_TIMER_1, TRUE, TRUE);
	vAHI_TimerFineGrainDIOControl(0x7);

	//ADC
	vAHI_ApConfigure(E_AHI_AP_REGULATOR_ENABLE, E_AHI_AP_INT_DISABLE,
	                 E_AHI_AP_SAMPLE_2, E_AHI_AP_CLOCKDIV_500KHZ, E_AHI_AP_INTREF);
	while( !bAHI_APRegulatorEnabled() ) {}
}


