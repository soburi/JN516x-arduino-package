#include "power_driver.h"
#include "platform.h"
#include <AppHardwareApi.h>
#include <stdbool.h>

#define MSEC2WTCOUNT(ms)  ( (ms)  * 32 )

uint32_t sleepmode = 0;
uint64_t sleepcount = 0;

static void sleep(uint32_t mode, uint32_t ms)
{
	sleepmode = mode;
	sleepcount = MSEC2WTCOUNT(ms);
}

void power_sleep(uint32_t millis)
{
	sleep(E_AHI_SLEEP_OSCON_RAMON, millis);
}

void power_deepsleep(uint32_t millis)
{
	sleep(E_AHI_SLEEP_OSCON_RAMOFF, millis);
}

void power_idle(uint32_t millis)
{
	(void)millis;
}
