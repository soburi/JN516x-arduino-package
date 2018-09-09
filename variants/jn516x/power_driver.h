#ifndef POWER_DRIVER_H__
#define POWER_DRIVER_H__


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

extern void power_sleep(void* dev, uint32_t millis);

extern void power_deepsleep(void* dev, uint32_t millis);

extern void power_idle(void* dev, uint32_t millis);

extern uint32_t power_wakeup_reason(void* dev);

#ifdef __cplusplus
}
#endif

#endif
