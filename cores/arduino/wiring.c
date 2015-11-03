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

#include "Arduino.h"

#include "wiring_private.h"

#include <contiki.h>
#include <net/netstack.h>

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

extern uint32_t heap_location;

void init()
{
	/* Set stack overflow address for detecting overflow in runtime */
	vAHI_SetStackOverflow(TRUE, ((uint32_t *)&heap_location)[0]);

	clock_init();

	vAHI_StartRandomNumberGenerator(E_AHI_RND_SINGLE_SHOT, E_AHI_INTS_DISABLED);
	random_init(u16AHI_ReadRandomNumber());

	process_init();
	ctimer_init();

	/* check for reset source */
	if(bAHI_WatchdogResetEvent()) {
		printf("Init: Watchdog timer has reset device!\r\n");
	}
	process_start(&etimer_process, NULL);
	netstack_init();	
	NETSTACK_LLSEC.init();
}
