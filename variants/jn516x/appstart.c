#include <stdint.h>

#include "Arduino.h"

extern int main(void);

typedef void (*funcptr)(); 
extern funcptr ctors_start;

void AppColdStart(void)
{
	// call global initializers.
	unsigned long* ptr = (unsigned long*)(&ctors_start);

	// Terminate with .end_ctors section header.
	while(*ptr) {
		funcptr fp = (funcptr)(*ptr++);
		fp();
	}
	main();
}

void AppWarmStart(void)
{
	main();
}


