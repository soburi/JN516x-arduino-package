#pragma once

#ifdef __cplusplus
extern "C"{
#endif

#include "platform.h"

#include "WInterrupts.h"
#include "wiring_constants.h"

#define DIO_NUM 20

extern uint32_t ticktimer_overflow_count;
extern voidFuncPtr handler_table[DIO_NUM];

extern void ticktimer_callback(uint32_t u32Device, uint32_t u32ItemBitmap);
extern void sysctrl_callback(uint32_t u32Device, uint32_t u32ItemBitmap);

#ifdef __cplusplus
} // extern "C"
#endif
