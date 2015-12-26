/*
  Copyright (c) TOKITA Hiroshi.  All right reserved.

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

#include "Arduino.h"

#include <dev/leds.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t gpioPin2LedBits(uint32_t ulPin) __attribute__((weak));
uint32_t gpioPin2LedBits(uint32_t ulPin)
{
	return ulPin;
}

void _pinModeDefault( uint32_t ulPin, uint32_t ulMode )
{
}


void _digitalWriteDefault( uint32_t ulPin, uint32_t ulVal )
{
	if(ulVal == LOW)
	{
		leds_off( gpioPin2LedBits(ulPin) );
	}
	else
	{
		leds_on( gpioPin2LedBits(ulPin) );
	}
}


int _digitalReadDefault( uint32_t ulPin )
{
	if( leds_get() & gpioPin2LedBits(ulPin) )
	{
		return HIGH;
	}
	
	return LOW;
}

void pinMode( uint32_t ulPin, uint32_t ulMode ) __attribute__((weak, alias("_pinModeDefault")));
void digitalWrite( uint32_t ulPin, uint32_t ulVal ) __attribute__((weak, alias("_digitalWriteDefault")));
int digitalRead( uint32_t ulPin ) __attribute__((weak, alias("_digitalReadDefault")));
#ifdef __cplusplus
}
#endif

