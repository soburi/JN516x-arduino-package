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
#include <AppHardwareApi.h>

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

#define TONE_INTERVAL_OFFSET(freq) (-((RTIMER_SECOND / 1000000) * 8))

typedef uint8_t IRQn_Type;

typedef uint8_t Pio;
//typedef enum { PioType_NONE } EPioType;

#define DIO_NUM 20

extern void DIO_interrupt_handler(uint32_t device, uint32_t bits);

extern uint8_t segmentLength;
extern uint16_t segmentNumber;

static inline void eeprom_init()
{
	segmentNumber = u16AHI_InitialiseEEP(&segmentLength);
	DBG_PRINTF("u16AHI_InitialiseEEP() num:%d, len:%d\n", segmentNumber, segmentLength);
}

static inline uint8_t eeprom_read_byte(const uint8_t* index)
{
	uint8_t buf;
	int ret = iAHI_ReadDataFromEEPROMsegment( ((uint32_t)index)/segmentLength,
						  ((uint32_t)index)%segmentLength, &buf, 1);
	DBG_PRINTF("iAHI_ReadDataFromEEPROMsegment(%d, %d) ret:%d, buf:%x\n",
			((uint32_t)index)/segmentLength,
			((uint32_t)index)%segmentLength, ret, buf);
	return buf;
}

static inline void eeprom_write_byte(const uint8_t* index, uint8_t data)
{
	int ret = iAHI_WriteDataIntoEEPROMsegment(((uint32_t)index)/segmentLength,
						  ((uint32_t)index)%segmentLength, &data, 1);
	DBG_PRINTF("iAHI_WriteDataFromEEPROMsegment(%d, %d) ret:%d\n",
			((uint32_t)index)/segmentLength,
			((uint32_t)index)%segmentLength, ret);
}

static inline size_t eeprom_size() {
	return segmentLength * segmentNumber;
}

#ifdef __cplusplus
}
#endif

#endif //PLATFORM_DEPEND_INCLUDES_H
