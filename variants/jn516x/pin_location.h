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

#ifndef PIN_LOCATION_H__
#define PIN_LOCATION_H__

#include <contiki.h>

/***********************************************
 * bit  0-19  DIO[x]
 * bit 20,21  DO0,  DO1
 * bit 22,23 ADC1, ADC2
 ***********************************************/

#define USE_JTAG			0x0000F000
#define USE_JTAG_ALT			0x000000F0
#define USE_UART_0			0x000000C0
#define USE_UART_0_CTSRTS		0x000000F0
#define USE_UART_0_ALT			0x0000C000
#define USE_UART_0_CTSRTS_ALT		0x0000F000
#define USE_UART_1			0x00000A00
#define USE_UART_1_ALT			0x0000C000
#define USE_TIMER_0			0x00000400
#define USE_TIMER_0_CAPTURE		0x00000600
#define USE_TIMER_0_CLOCK_GATE		0x00000500
#define USE_TIMER_0_ALT			0x00000010
#define USE_TIMER_0_CAPTURE_ALT		0x00000018
#define USE_TIMER_0_CLOCK_GATE_ALT	0x00000014
#define USE_TIMER_1			0x00000800
#define USE_TIMER_1_ALT			0x00000020
#define USE_TIMER_2			0x00001000
#define USE_TIMER_2_ALT			0x00000040
#define USE_TIMER_2_RELOC		0x00100000
#define USE_TIMER_3			0x00002000
#define USE_TIMER_3_ALT			0x00000080
#define USE_TIMER_3_RELOC		0x00200000
#define USE_TIMER_4			0x00004000
#define USE_TIMER_4_ALT			0x00000100
#define USE_PC_0			0x00000020
#define USE_PC_0_ALT			0x00000010
#define USE_PC_1			0x00000100
#define USE_PC_1_ALT			0x00000002
#define USE_SI				0x0000C000
#define USE_SI_ALT			0x00030000
#define USE_SPIM			0x003C0000
#define USE_SPIM_SEL_1			0x003C0001
#define USE_SPIM_SEL_1_SEL_2		0x003C4000
#define USE_SPIM_SEL_1_SEL_2_ALT	0x003C8001
#define USE_SPIM_SEL_1_ALT		0x003C0003
#define USE_SPIM_SEL_1_ALT_SEL_2	0x003CC000
#define USE_SPIM_SEL_1_ALT_SEL_2_ALT	0x003C4002
#define USE_SPIS			0x0003C000
#define USE_SPIS_ALT			0x0000F000
#define USE_ADC_1			0x00400000
#define USE_ADC_2			0x00800000
#define USE_ADC_3			0x00000001
#define USE_ADC_4			0x00000002
#define USE_COMP			0x00030000
#define USE_VREF			0x00800000
#define USE_ANT_DIV			0x0000C000
#define USE_HIGHPOWER_MOD		0x0000000C

#define USE_DIO_1			0x00000001
#define USE_DIO_2			0x00000002
#define USE_DIO_3			0x00000004
#define USE_DIO_4			0x00000008
#define USE_DIO_5			0x00000010
#define USE_DIO_6			0x00000020
#define USE_DIO_7			0x00000040
#define USE_DIO_8			0x00000080
#define USE_DIO_9			0x00000100
#define USE_DIO_10			0x00000200
#define USE_DIO_11			0x00000400
#define USE_DIO_12			0x00000800
#define USE_DIO_13			0x00001000
#define USE_DIO_14			0x00002000
#define USE_DIO_15			0x00004000
#define USE_DIO_16			0x00008000
#define USE_DIO_17			0x00010000
#define USE_DIO_18			0x00020000
#define USE_DIO_19			0x00040000

#define NOT_USE 			0x00000000

#ifndef PIN_JTAG
#define PIN_JTAG NOT_USE
#endif

#ifndef PIN_UART_0
#define PIN_UART_0 NOT_USE
#endif

#ifndef PIN_UART_1
#define PIN_UART_1 NOT_USE
#endif

#ifndef PIN_TIMER_0
#define PIN_TIMER_0 NOT_USE
#endif

#ifndef PIN_TIMER_1
#define PIN_TIMER_1 NOT_USE
#endif

#ifndef PIN_TIMER_2
#define PIN_TIMER_2 NOT_USE
#endif

#ifndef PIN_TIMER_3
#define PIN_TIMER_3 NOT_USE
#endif

#ifndef PIN_TIMER_4
#define PIN_TIMER_4 NOT_USE
#endif

#ifndef PIN_PC_0
#define PIN_PC_0 NOT_USE
#endif

#ifndef PIN_PC_1
#define PIN_PC_1 NOT_USE
#endif

#ifndef PIN_SI
#define PIN_SI NOT_USE
#endif

#ifndef PIN_SPIM
#define PIN_SPIM NOT_USE
#endif

#ifndef PIN_SPIS
#define PIN_SPIS NOT_USE
#endif

#ifndef PIN_ADC_1
#define PIN_ADC_1 NOT_USE
#endif

#ifndef PIN_ADC_2
#define PIN_ADC_2 NOT_USE
#endif

#ifndef PIN_ADC_3
#define PIN_ADC_3 NOT_USE
#endif

#ifndef PIN_ADC_4
#define PIN_ADC_4 NOT_USE
#endif

#ifndef PIN_COMP
#define PIN_COMP NOT_USE
#endif

#ifndef PIN_VREF
#define PIN_VREF NOT_USE
#endif

#ifndef PIN_ANT_DIV
#define PIN_ANT_DIV NOT_USE
#endif

#ifndef PIN_HIGHPOWER_MOD
#define PIN_HIGHPOWER_MOD NOT_USE
#endif

#ifndef PIN_DIO_0
#define PIN_DIO_0 NOT_USE
#endif

#ifndef PIN_DIO_1
#define PIN_DIO_1 NOT_USE
#endif

#ifndef PIN_DIO_2
#define PIN_DIO_2 NOT_USE
#endif

#ifndef PIN_DIO_3
#define PIN_DIO_3 NOT_USE
#endif

#ifndef PIN_DIO_4
#define PIN_DIO_4 NOT_USE
#endif

#ifndef PIN_DIO_5
#define PIN_DIO_5 NOT_USE
#endif

#ifndef PIN_DIO_6
#define PIN_DIO_6 NOT_USE
#endif

#ifndef PIN_DIO_7
#define PIN_DIO_7 NOT_USE
#endif

#ifndef PIN_DIO_8
#define PIN_DIO_8 NOT_USE
#endif

#ifndef PIN_DIO_9
#define PIN_DIO_9 NOT_USE
#endif

#ifndef PIN_DIO_10
#define PIN_DIO_10 NOT_USE
#endif

#ifndef PIN_DIO_11
#define PIN_DIO_11 NOT_USE
#endif

#ifndef PIN_DIO_12
#define PIN_DIO_12 NOT_USE
#endif

#ifndef PIN_DIO_13
#define PIN_DIO_13 NOT_USE
#endif

#ifndef PIN_DIO_14
#define PIN_DIO_14 NOT_USE
#endif

#ifndef PIN_DIO_15
#define PIN_DIO_15 NOT_USE
#endif

#ifndef PIN_DIO_16
#define PIN_DIO_16 NOT_USE
#endif

#ifndef PIN_DIO_17
#define PIN_DIO_17 NOT_USE
#endif

#ifndef PIN_DIO_18
#define PIN_DIO_18 NOT_USE
#endif

#ifndef PIN_DIO_19
#define PIN_DIO_19 NOT_USE
#endif

#ifndef PIN_DIO_20
#define PIN_DIO_20 NOT_USE
#endif

#define PIN_BITMAP (PIN_JTAG + PIN_UART_0 + PIN_UART_1 + PIN_TIMER_0 + PIN_TIMER_1 + PIN_TIMER_2 + \
     PIN_TIMER_3 + PIN_TIMER_4 + PIN_PC_0 + PIN_PC_1 + PIN_SI + PIN_SPIM + \
     PIN_SPIS + PIN_ADC_1 + PIN_ADC_2 + PIN_ADC_3 + PIN_ADC_4 + PIN_COMP + \
     PIN_VREF + PIN_ANT_DIV + PIN_HIGHPOWER_MOD + \
     PIN_DIO_0 + PIN_DIO_1 + PIN_DIO_2 + PIN_DIO_3 + PIN_DIO_4 + PIN_DIO_5 + \
     PIN_DIO_6 + PIN_DIO_7 + PIN_DIO_8 + PIN_DIO_9 + PIN_DIO_10 + PIN_DIO_11 + \
     PIN_DIO_12 + PIN_DIO_13 + PIN_DIO_14 + PIN_DIO_15 + PIN_DIO_16 + PIN_DIO_17 + \
     PIN_DIO_18 + PIN_DIO_19 + PIN_DIO_20)

#endif //PIN_LOCATION_H__

