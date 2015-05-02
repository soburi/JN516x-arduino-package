#include <stdint.h>

#include "Arduino.h"

extern int main(void);

extern "C" {
	typedef void (*funcptr)(); 
	extern funcptr ctors_start;
}

extern "C"  void AppColdStart(void)
{
	// call global initializers.
	funcptr fp = ctors_start;
	fp();

	main();
}

extern "C" void AppWarmStart(void)
{
	AppColdStart();
}


