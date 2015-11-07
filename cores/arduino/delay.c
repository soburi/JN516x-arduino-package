#include "delay.h"
#include "Arduino.h"

#include "platform.h"

#define MSEC2TIMERCOUNT(ms) (ms*125)

#define CLOCK_TIMER E_AHI_TIMER_1
#define CLOCK_INTERVAL (125 * 10)
#define MAX_TICKS (CLOCK_INTERVAL)

uint32_t millis( void )
{
	return clock_seconds() * 1000 + (RTIMER_NOW() % RTIMER_ARCH_SECOND) / (RTIMER_ARCH_SECOND/1000);
}

uint32_t micros( void )
{
	return RTIMER_NOW()/(RTIMER_ARCH_SECOND/1000000);
}

void delay( uint32_t ms )
{
  unsigned int i = MSEC2TIMERCOUNT(ms);
  volatile clock_time_t start_tick = clock_time();
  volatile uint32_t t = u16AHI_TimerReadCount(CLOCK_TIMER);

  /* beware of wrapping */
  if(MAX_TICKS - t < i) {
    while(u16AHI_TimerReadCount(CLOCK_TIMER) < MAX_TICKS && u16AHI_TimerReadCount(CLOCK_TIMER) != 0) ;
    i -= MAX_TICKS - t;
    t = 0;
  }
  while(u16AHI_TimerReadCount(CLOCK_TIMER) - t < i && clock_time() - start_tick < (i/CLOCK_INTERVAL) ) {
    watchdog_periodic();
  }
}
