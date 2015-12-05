#include "delay.h"
#include "Arduino.h"

#include "platform.h"

#include "wiring_private.h"

uint32_t millis( void )
{
	return clock_seconds() * 1000 + (RTIMER_NOW() % RTIMER_ARCH_SECOND) * 1000 / RTIMER_ARCH_SECOND;
}

uint32_t micros( void )
{
	return RTIMER_NOW() * 1000000/RTIMER_ARCH_SECOND;
}

static struct etimer delay_timer;

static void delay_timer_start(void* data)
{
	uint32_t ms = *(uint32_t*)data;
	etimer_set(&delay_timer, CLOCK_SECOND * ms /1000);
}

static int delay_timer_expired(process_event_t ev, process_data_t data, void* param)
{
	(void)ev; (void)data; (void)param;
	return etimer_expired(&delay_timer);
}

void delay( uint32_t ms )
{
	main_thread_wait.type = WT_YIELD_UNTIL;
	main_thread_wait.run_param = &ms;
	main_thread_wait.run = delay_timer_start;
	main_thread_wait.condition = delay_timer_expired;
	mt_yield();
}
