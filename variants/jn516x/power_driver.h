#ifndef POWER_DRIVER_H__
#define POWER_DRIVER_H__


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

extern void power_sleep(uint32_t millis);

extern void power_deepsleep(uint32_t millis);

extern void power_idle(uint32_t millis);


#ifdef __cplusplus
}
#endif

#endif
