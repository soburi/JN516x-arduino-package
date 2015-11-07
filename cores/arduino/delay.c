#include "delay.h"
#include "Arduino.h"

#include "wiring_private.h"
#define clock2usec(a) ( (a) / clockCyclesPerMicrosecond() )
#define MICROSECONDS_PER_TICKTIMER_OVERFLOW (clock2usec(0x0FFFFFFF))
#define MILLIS_INC (MICROSECONDS_PER_TICKTIMER_OVERFLOW / 1000)
#define TICK_TIMER_MAX (0x0fffffff)

unsigned long millis()
{
    unsigned long m;
    m = MILLIS_INC * ticktimer_overflow_count +
        (clock2usec(u32AHI_TickTimerRead())/1000);
    DBG_PRINTF("millis: ");
    DBG_PRINTF("%d\r\n", m);
    DBG_PRINTF("\r\n");
    return m;
}

unsigned long micros() {
    unsigned long m;

    m = MICROSECONDS_PER_TICKTIMER_OVERFLOW * ticktimer_overflow_count +
        clock2usec(u32AHI_TickTimerRead());
    return m;
}

void delay(unsigned long ms)
{
    uint32_t start = (uint32_t)micros();

    while (ms > 0) {
        yield();
        if (((uint32_t)micros() - start) >= 1000) {
            ms--;
            start += 1000;
        }
    }

    DBG_PRINTF("delay: ");
    DBG_PRINTF("%d\r\n", micros());
    DBG_PRINTF("\r\n");
}


