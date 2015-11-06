/*
  Copyright (c) 2014 Arduino.  All right reserved.

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

#include "platform.h"

#include "watchdog.h"
#include <AppHardwareApi.h>
#include <math.h>

void watchdogEnable (uint32_t timeout)
{
	uint8_t prescale = (uint8_t)log2(timeout/8-1)+1;
	prescale += 1; //roundup
	prescale = prescale <  0 ?  0 : prescale;
	prescale = prescale > 12 ? 12 : prescale;

	vAHI_WatchdogStart(prescale);
}

void watchdogDisable(void)
{
	vAHI_WatchdogStop();
}

void watchdogReset(void)
{
	vAHI_WatchdogRestart();
}


extern "C"
void _watchdogDefaultSetup (void)
{
	vAHI_WatchdogStop();
}
void watchdogSetup (void) __attribute__ ((weak, alias("_watchdogDefaultSetup")));


