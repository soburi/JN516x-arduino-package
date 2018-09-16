#include <stdbool.h>
#include <stdint.h>
#include "power-driver.h"
#include "platform.h"
#include <AppHardwareApi.h>
#include <sys/process.h>
#include <wiring_private.h>

#define MSEC2WTCOUNT(ms)  ( (ms)  * 32 )

extern uint32_t sleep_mode;
extern uint64_t sleep_count;
extern uint32_t wake_gpio;
extern uint8_t wake_timer;
extern uint8_t wake_comparator;

static void nop(void* x) { (void)x; }

static int wait_infinity(process_event_t ev, process_data_t data, void* param)
{
	(void)data; (void)param;
	return ev == waketimer_event ? 1 : 0;
}


static void sleep(uint32_t mode, uint32_t ms)
{
	printf("power_sleep %d %d\n", mode, ms);
	sleep_mode = mode;
	sleep_count = MSEC2WTCOUNT(ms);
	yield_until(nop, NULL, wait_infinity, NULL);
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

	printf("power_wakeup_reason %u %u %u\n", wake_gpio, wake_timer, wake_comparator);

	if(wake_comparator != 0) return 3;
	if(wake_timer != 0) return 4;
	if(wake_gpio != 0) return 1;

	return 0;
}

void power_enable_wake_gpio(void* dev, bool enable, uint32_t type, uint32_t x, uint32_t y)
{
}

