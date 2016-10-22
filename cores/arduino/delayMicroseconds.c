#include <sys/clock.h>

void _delayMicrosecondsDefault( uint32_t usec )
{
	clock_delay_usec(usec);
}

void delayMicroseconds( uint32_t usec ) __attribute__((weak,alias("_delayMicrosecondsDefault")));
