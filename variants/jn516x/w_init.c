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

#include "platform.h"


/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */

extern uint32_t heap_location;

void initVariant( void )
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
