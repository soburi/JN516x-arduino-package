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

#include <stdint.h>
#include "Arduino.h"
#include "wiring_private.h"

static uint32_t pulsein_pin;
static uint32_t pulsein_state;
static uint32_t pulse_start_time;
static uint32_t pulse_end_time;

static void pulse_end_callback()
{
	pulse_end_time = micros();
}

static void pulse_start_callback()
{
	pulse_start_time = micros();
	attachInterrupt(pulsein_pin, pulse_end_callback, pulsein_state == HIGH ? FALLING : RISING);
}

uint32_t _pulseInDefault(uint32_t pin, uint32_t state, uint32_t timeout)
{
	DBG_PRINTF("_pulseInDefault(%d, %d, %d)\r\n", pin, state, timeout);
	pulse_start_time = 0;
	pulse_end_time = 0;
	pulsein_pin = pin;
	pulsein_state = state;
	attachInterrupt(pulsein_pin, pulse_start_callback, pulsein_state == HIGH ? RISING : FALLING);
	clock_delay_usec(timeout);
	uint32_t end = timeout + micros();
	while (micros() < end && pulse_end_time == 0) {
		yield();
	}
	detachInterrupt(pulsein_pin);

	DBG_PRINTF("end: %d start: %d width: %d)\r\n", pulse_end_time, pulse_start_time, pulse_end_time - pulse_start_time);
	if(pulse_end_time != 0) {
		return pulse_end_time - pulse_start_time;
	}

	return 0;
}

uint32_t pulseIn(uint32_t pin, uint32_t state, uint32_t timeout) __attribute__ ((weak, alias("_pulseInDefault")) );
