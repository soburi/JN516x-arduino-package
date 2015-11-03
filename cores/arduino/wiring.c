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

#define USEC2TICKCOUNT(us) (us*16)
#define MSEC2TIMERCOUNT(ms) (ms*125)

#define CLOCK_TIMER E_AHI_TIMER_1
#define CLOCK_INTERVAL (125 * 10)
#define MAX_TICKS (CLOCK_INTERVAL)

unsigned long millis()
{
	return clock_seconds() * 1000 + (RTIMER_NOW() % RTIMER_ARCH_SECOND) / (RTIMER_ARCH_SECOND/1000);
}

unsigned long micros() {
	return RTIMER_NOW()/(RTIMER_ARCH_SECOND/1000000);
}

void delay(unsigned long ms)
{
  unsigned int i = MSEC2TIMERCOUNT(ms);
  volatile clock_time_t start_tick = clock_time();
  volatile uint32_t t = u16AHI_TimerReadCount(CLOCK_TIMER);

  /* beware of wrapping */
  if(MAX_TICKS - t < i) {
    while(u16AHI_TimerReadCount(CLOCK_TIMER) < MAX_TICKS && u16AHI_TimerReadCount(CLOCK_TIMER) != 0) ;
    i -= MAX_TICKS - t;
    t = 0;
  }
  while(u16AHI_TimerReadCount(CLOCK_TIMER) - t < i && clock_time() - start_tick < (i/CLOCK_INTERVAL) ) {
    watchdog_periodic();
  }
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
