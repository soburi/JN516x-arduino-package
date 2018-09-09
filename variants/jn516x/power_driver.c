#include <stdbool.h>
#include <stdint.h>
#include "power_driver.h"
#include "platform.h"
#include <AppHardwareApi.h>

#define MSEC2WTCOUNT(ms)  ( (ms)  * 32 )

extern uint32_t sleep_mode;
extern uint64_t sleep_count;
extern uint32_t wake_gpio;
extern uint8_t wake_timer;
extern uint8_t wake_comparator;

static void sleep(uint32_t mode, uint32_t ms)
{
	sleep_mode = mode;
	sleep_count = MSEC2WTCOUNT(ms);
}

void power_sleep(void* dev, uint32_t millis)
{
	(void)dev;
	sleep(E_AHI_SLEEP_OSCON_RAMOFF, millis);
}

void power_deepsleep(void* dev, uint32_t millis)
{
	(void)dev;
	sleep(E_AHI_SLEEP_DEEP, millis);
}

void power_idle(void* dev, uint32_t millis)
{
	(void)dev;
	sleep(E_AHI_SLEEP_OSCON_RAMON, millis);
}

uint32_t power_wakeup_reason(void* dev)
{
	(void)dev;
	if(wake_gpio != 0) return 1;
	if(wake_timer != 0) return 4;
	if(wake_comparator != 0) return 3;

	return 0;
}
