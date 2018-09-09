#include <stdbool.h>
#include <stdint.h>
#include "power_driver.h"
#include "platform.h"
#include <AppHardwareApi.h>

#define MSEC2WTCOUNT(ms)  ( (ms)  * 32 )

extern uint32_t sleep_mode;
extern uint64_t sleep_count;

static void sleep(uint32_t mode, uint32_t ms)
{
	sleep_mode = mode;
	sleep_count = MSEC2WTCOUNT(ms);
}

void power_sleep(uint32_t millis)
{
	sleep(E_AHI_SLEEP_OSCON_RAMOFF, millis);
}

void power_deepsleep(uint32_t millis)
{
	sleep(E_AHI_SLEEP_DEEP, millis);
}

void power_idle(uint32_t millis)
{
	sleep(E_AHI_SLEEP_OSCON_RAMON, millis);
}

void power_enable_wake_gpio(bool enable, uint32_t pin, uint32_t mode)
{
	(void)enable;
	(void)pin;
	(void)mode;
}
