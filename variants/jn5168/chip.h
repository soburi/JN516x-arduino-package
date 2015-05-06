#ifndef _VARIANT_CHIP_H_
#define _VARIANT_CHIP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <jendefs.h>
#include <MicroSpecific.h>

#define SystemCoreClock F_INTERNAL_SYSCLOCK

#define __disable_irq() MICRO_DISABLE_INTERRUPTS()
#define __enable_irq()  MICRO_ENABLE_INTERRUPTS()

#ifdef __cplusplus
}
#endif

#endif //_VARIANT_CHIP_H_
