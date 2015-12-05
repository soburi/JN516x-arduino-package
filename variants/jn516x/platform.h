#ifndef _VARIANT_PLATFORM_H
#define _VARIANT_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#undef F_CPU

#include <contiki.h>
#include <net/netstack.h>
#include <dev/watchdog.h>
#include <dev/uart0.h>
#include <dev/uart1.h>
#include <lib/random.h>

#include <jendefs.h>
#include <MicroSpecific.h>

#define F_INTERNAL_SYSCLOCK (F_CPU/2)

#define SystemCoreClock F_INTERNAL_SYSCLOCK

#define __disable_irq() MICRO_DISABLE_INTERRUPTS()
#define __enable_irq()  MICRO_ENABLE_INTERRUPTS()

typedef uint8_t IRQn_Type;

typedef uint8_t Pio;
//typedef enum { PioType_NONE } EPioType;

#define DIO_NUM 20

typedef void (*void_fp_void)(void);

extern void_fp_void handler_table[DIO_NUM];
extern uint32_t ticktimer_overflow_count;

extern void ticktimer_callback(uint32_t u32Device, uint32_t u32ItemBitmap);
extern void sysctrl_callback(uint32_t u32Device, uint32_t u32ItemBitmap);

#ifdef __cplusplus
}
#endif

#endif //PLATFORM_DEPEND_INCLUDES_H
