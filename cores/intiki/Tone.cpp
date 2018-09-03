/*
  Copyright (c) 2017 Tokita, Hiroshi  All right reserved.

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

#include <stdint.h>
#include "wiring_digital.h"
#include "wiring_constants.h"
#include "platform.h"

extern "C" {
#include <contiki.h>
}

#ifndef TONE_INTERVAL_OFFSET
#define TONE_INTERVAL_OFFSET(f) 0
#endif

void tone(uint32_t _pin, uint32_t frequency, uint32_t duration) __attribute__((weak));
void noTone(uint32_t _pin) __attribute__((weak));

static uint32_t output_pin;
static uint32_t output_state;
static uint32_t interval;
static uint32_t count;
static struct rtimer tone_rtimer;

static void rtimer_callback(struct rtimer* /*timer*/, void* /*ptr*/)
{
	//DBG_PRINTF("rtimer_callback()\r\n");
	if(count > 0) {
		count--;
	}
	else if(count == 0) {
		digitalWrite(output_pin, LOW);
		return;
	}
	output_state = (output_state == HIGH ? LOW : HIGH);
	digitalWrite(output_pin, output_state);
	rtimer_clock_t next = RTIMER_NOW() + interval;
	rtimer_set(&tone_rtimer, next, 0, rtimer_callback, NULL);
}

void tone(uint32_t pin, uint32_t frequency, uint32_t duration)
{
	DBG_PRINTF("tone(%d, %d, %d)\r\n", pin, frequency, duration);
	output_pin = pin;
	output_state = LOW;
	digitalWrite(output_pin, output_state);
	interval = (RTIMER_SECOND / (frequency*2)) + TONE_INTERVAL_OFFSET(frequency);
	if(duration == 0) {
		count = -1;
	}
	else {
		count = frequency * 2 * duration / 1000;
	}
	rtimer_clock_t next = RTIMER_NOW() + interval;
	DBG_PRINTF("rtimer_set(%d) interval:%d count:%d\r\n", next, 0, count);
	rtimer_set(&tone_rtimer, next, 0, rtimer_callback, NULL);
}

void noTone(uint32_t pin)
{
	count = 0;
	digitalWrite(pin, LOW);
}

