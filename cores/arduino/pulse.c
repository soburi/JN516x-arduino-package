#include <watchdog.h>

uint32_t _pulseInDefault(uint32_t pin, uint32_t state, uint32_t timeout)
{
	return 0;
}

uint32_t pulseIn(uint32_t pin, uint32_t state, uint32_t timeout) __attribute__ ((weak, alias("_pulseInDefault")) );
