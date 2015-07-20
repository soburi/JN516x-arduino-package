#include <stdint.h>

#include "Arduino.h"

#include <dbg.h>

#ifndef DBG_UART
#define DBG_UART DBG_E_UART_0
#endif
#ifndef DBG_BAUD_RATE
#define DBG_BAUD_RATE DBG_E_UART_BAUD_RATE_9600
#endif

extern int main(void);

extern "C" {
	typedef void (*funcptr)(); 
	extern funcptr ctors_start;
}

extern "C"  void AppColdStart(void)
{
#ifdef DBG_ENABLE
	DBG_vUartInit(DBG_UART, DBG_BAUD_RATE);
#endif
	DBG_PRINTF("AppColdStart\r\n");
	// call global initializers.
	unsigned long* ptr = reinterpret_cast<unsigned long*>(&ctors_start);

	// Terminate with .end_ctors section header.
	while(*ptr) {
		funcptr fp = reinterpret_cast<funcptr>(*ptr++);
		fp();
	}
	main();
}

extern "C" void AppWarmStart(void)
{
#ifdef DBG_ENABLE
	DBG_vUartInit(DBG_UART, DBG_BAUD_RATE);
	for(int i=0; i<1000; i++) ; //stabilize uart.
#endif
	DBG_PRINTF("AppWarmStart\r\n");
	main();
}


