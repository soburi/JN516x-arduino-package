/*
  wiring_digital.c - digital input and output functions
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

  Modified 28 September 2010 by Mark Sproul

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $
*/
#include "Arduino.h"
#include "wiring_private.h"

#include <AppHardwareApi.h>

void pinMode(uint32_t pin, uint32_t mode)
{
	if (mode == INPUT) {
		vAHI_DioSetDirection((1UL<<pin), 0);
		vAHI_DioSetPullup(0, (1UL<<pin));
		DBG_PRINTF("input  p:");
		DBG_PRINTF("%d\r\n", pin);
		DBG_PRINTF(" m:");
		DBG_PRINTF("%d\r\n", mode);
		DBG_PRINTF("\r\n");
	} else if (mode == INPUT_PULLUP) {
		vAHI_DioSetDirection(0, (1UL<<pin));
		vAHI_DioSetPullup((1UL<<pin), 0 );
		DBG_PRINTF("pullup p:");
		DBG_PRINTF("%d\r\n", pin);
		DBG_PRINTF(" m:");
		DBG_PRINTF("%d\r\n", mode);
		DBG_PRINTF("\r\n");

	} else {
		vAHI_DioSetDirection(0, (1UL<<pin) );
		DBG_PRINTF("output p:");
		DBG_PRINTF("%d\r\n", pin);
		DBG_PRINTF(" m:");
		DBG_PRINTF("%d\r\n", mode);
		DBG_PRINTF("\r\n");
	}
}


// Forcing this inline keeps the callers from having to push their own stuff
// on the stack. It is a good performance win and only takes 1 more byte per
// user than calling. (It will take more bytes on the 168.)
//
// But shouldn't this be moved into pinMode? Seems silly to check and do on
// each digitalread or write.
//
// Mark Sproul:
// - Removed inline. Save 170 bytes on atmega1280
// - changed to a switch statment; added 32 bytes but much easier to read and maintain.
// - Added more #ifdefs, now compiles for atmega645
//
//static inline void turnOffPWM(uint8_t timer) __attribute__ ((always_inline));
//static inline void turnOffPWM(uint8_t timer)
//static void turnOffPWM(uint8_t timer)
//{
//}

void digitalWrite(uint32_t pin, uint32_t val)
{
	if (val == LOW) {
		DBG_PRINTF("LOW  p:");
		DBG_PRINTF("%d\r\n", pin);
		DBG_PRINTF(" v:");
		DBG_PRINTF("%d\r\n", val);
		DBG_PRINTF("\r\n");
		vAHI_DioSetOutput(0, (1UL<<pin));
	} else {
		DBG_PRINTF("HIGH p:");
		DBG_PRINTF("%d\r\n", pin);
		DBG_PRINTF(" v:");
		DBG_PRINTF("%d\r\n", val);
		DBG_PRINTF("\r\n");
		vAHI_DioSetOutput((1UL<<pin), 0);
	}
}

int digitalRead(uint32_t pin)
{
	if (u32AHI_DioReadInput() & (1<<pin)) return HIGH;
	return LOW;
}
