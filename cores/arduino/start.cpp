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
	main();
}


