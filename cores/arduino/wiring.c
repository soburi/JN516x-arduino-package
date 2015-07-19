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

#define ARDUINO_MAIN
#include "Arduino.h"

#include "wiring_private.h"


#define clock2usec(a) ( (a) / clockCyclesPerMicrosecond() )
#define MICROSECONDS_PER_TICKTIMER_OVERFLOW (clock2usec(0x0FFFFFFF))
#define MILLIS_INC (MICROSECONDS_PER_TICKTIMER_OVERFLOW / 1000)
#define TICK_TIMER_MAX (0x0fffffff)

unsigned long millis()
{
	unsigned long m;
	m = MILLIS_INC * ticktimer_overflow_count +
		(clock2usec(u32AHI_TickTimerRead())/1000);
	DBG_PRINTF("millis: ");
	DBG_PRINTF("%d\r\n", m);
	DBG_PRINTF("\r\n");
	return m;
}

unsigned long micros() {
	unsigned long m;

	m = MICROSECONDS_PER_TICKTIMER_OVERFLOW * ticktimer_overflow_count +
		clock2usec(u32AHI_TickTimerRead());
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

	DBG_PRINTF("delay: ");
	DBG_PRINTF("%d\r\n", micros());
	DBG_PRINTF("\r\n");
}

/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */

void init()
{
	u32AHI_Init();

	//WatchDog
	vAHI_WatchdogStop();

	//SysCtrl
	vAHI_SysCtrlRegisterCallback(sysctrl_callback);
#if !defined(DBG_ENABLE) || ( defined(DBG_UART) && (DBG_UART != DBG_E_UART_0) )
	vAHI_UartDisable(E_AHI_UART_0);
#endif
#if !defined(DBG_ENABLE) || ( DBG_UART != DBG_E_UART_1 )
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
	vAHI_TimerSetLocation(E_AHI_TIMER_1, true, true);
	vAHI_TimerSetLocation(E_AHI_TIMER_2, true, true);
	vAHI_TimerSetLocation(E_AHI_TIMER_3, true, true);
	vAHI_TimerSetLocation(E_AHI_TIMER_4, true, true);
	vAHI_TimerFineGrainDIOControl(0x87);

	//ADC
	vAHI_ApConfigure(E_AHI_AP_REGULATOR_ENABLE, E_AHI_AP_INT_DISABLE,
	                 E_AHI_AP_SAMPLE_2, E_AHI_AP_CLOCKDIV_500KHZ, E_AHI_AP_INTREF);
	while( !bAHI_APRegulatorEnabled() ) {}
}


