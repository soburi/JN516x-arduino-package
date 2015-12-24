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
#include "wiring_private.h"

#include <AppHardwareApi.h>

void pinMode( uint32_t ulPin, uint32_t ulMode )
{
	if (ulMode == INPUT) {
		vAHI_DioSetDirection((1UL<<ulPin), 0);
		vAHI_DioSetPullup(0, (1UL<<ulPin));
		DBG_PRINTF("input  p:");
		DBG_PRINTF("%d\r\n", ulPin);
		DBG_PRINTF(" m:");
		DBG_PRINTF("%d\r\n", ulMode);
		DBG_PRINTF("\r\n");
	} else if (ulMode == INPUT_PULLUP) {
		vAHI_DioSetDirection(0, (1UL<<ulPin));
		vAHI_DioSetPullup((1UL<<ulPin), 0 );
		DBG_PRINTF("pullup p:");
		DBG_PRINTF("%d\r\n", ulPin);
		DBG_PRINTF(" m:");
		DBG_PRINTF("%d\r\n", ulMode);
		DBG_PRINTF("\r\n");

	} else {
		vAHI_DioSetDirection(0, (1UL<<ulPin) );
		DBG_PRINTF("output p:");
		DBG_PRINTF("%d\r\n", ulPin);
		DBG_PRINTF(" m:");
		DBG_PRINTF("%d\r\n", ulMode);
		DBG_PRINTF("\r\n");
	}
}

void digitalWrite( uint32_t ulPin, uint32_t ulVal )
{
	if (ulVal == LOW) {
		DBG_PRINTF("LOW  p:");
		DBG_PRINTF("%d\r\n", ulPin);
		DBG_PRINTF(" v:");
		DBG_PRINTF("%d\r\n", ulVal);
		DBG_PRINTF("\r\n");
		vAHI_DioSetOutput(0, (1UL<<ulPin));
	} else {
		DBG_PRINTF("HIGH p:");
		DBG_PRINTF("%d\r\n", ulPin);
		DBG_PRINTF(" v:");
		DBG_PRINTF("%d\r\n", ulVal);
		DBG_PRINTF("\r\n");
		vAHI_DioSetOutput((1UL<<ulPin), 0);
	}
}

int digitalRead( uint32_t ulPin )
{
	if (u32AHI_DioReadInput() & (1<<ulPin)) return HIGH;
	return LOW;
}
