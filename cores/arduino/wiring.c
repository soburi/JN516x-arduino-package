#include "wiring.h"

void _initDefault()
{
	return;
}

void init(void) __attribute__ ((weak, alias("_initDefault")) );
