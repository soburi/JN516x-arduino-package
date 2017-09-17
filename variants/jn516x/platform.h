/*
  Copyright (c) TOKITA Hiroshi.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <dbg.h>
#include <dbg_uart.h>
#pragma GCC diagnostic pop

#define DBG_PRINTF(...) DBG_vPrintf(DBG_VPRINTF_ENABLE, __VA_ARGS__)
#ifdef DBG_ENABLE
#define DBG_VPRINTF_ENABLE true
#else
#define DBG_VPRINTF_ENABLE false
#endif

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

#define PLATFORM_SPI_HEADER "platform_spi.h"
#define PLATFORM_WIRE_HEADER "platform_wire.h"

#ifdef __cplusplus
}
#endif

#endif //PLATFORM_DEPEND_INCLUDES_H
