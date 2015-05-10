#include <stdint.h>

#include "Arduino.h"

extern int main(void);

extern "C" {
	typedef void (*funcptr)(); 
	extern funcptr ctors_start;
}

extern "C"  void AppColdStart(void)
{
#ifdef DBG_ENABLE

#ifndef DBG_UART
#define DBG_UART DBG_E_UART_0
#endif
#ifndef DBG_BAUD_RATE
#define DBG_BAUD_RATE DBG_E_UART_BAUD_RATE_9600
#endif
	DBG_vUartInit(DBG_UART, DBG_BAUD_RATE);
#endif
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
	AppColdStart();
}


