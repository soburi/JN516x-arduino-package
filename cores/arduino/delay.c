/*
  Copyright (c) 2016 TOKITA Hiroshi.  All right reserved.

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

#include "delay.h"
#include "Arduino.h"

#include "platform.h"

#include "wiring_private.h"

unsigned long millis( void )
{
	return clock_seconds() * 1000 + (RTIMER_NOW() % RTIMER_ARCH_SECOND) * 1000 / RTIMER_ARCH_SECOND;
}

// Interrupt-compatible version of micros
// Theory: repeatedly take readings of SysTick counter, millis counter and SysTick interrupt pending flag.
// When it appears that millis counter and pending is stable and SysTick hasn't rolled over, use these
// values to calculate micros. If there is a pending SysTick, add one to the millis counter in the calculation.
unsigned long micros( void )
{
	return RTIMER_NOW() * 1000000/RTIMER_ARCH_SECOND;
}

static struct etimer delay_timer;

static void delay_timer_start(void* data)
{
	uint32_t ms = *(uint32_t*)data;
	etimer_set(&delay_timer, CLOCK_SECOND * ms /1000);
}

static int delay_timer_expired(process_event_t ev, process_data_t data, void* param)
{
	(void)ev; (void)data; (void)param;
	return etimer_expired(&delay_timer);
}

void delay( unsigned long ms )
{
	yield_until(delay_timer_start, &ms, delay_timer_expired, NULL);
}
