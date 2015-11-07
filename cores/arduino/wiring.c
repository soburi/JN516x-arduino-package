#include "Arduino.h"

#include "wiring_private.h"

#include "platform.h"


/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */

extern uint32_t heap_location;

void init( void )
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
