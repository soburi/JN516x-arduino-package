/*
  Copyright (c) 2011 Arduino.  All right reserved.

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

#ifndef _VARIANT_
#define _VARIANT_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <dbg.h>
#include <dbg_uart.h>
#pragma GCC diagnostic pop

#include "Arduino.h"
#ifdef __cplusplus
#include "UARTClass.h"

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

extern UARTClass Serial;
extern UARTClass Serial1;
//extern USARTClass Serial2;
//extern USARTClass Serial3;

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR         Serial
//#define SERIAL_PORT_USBVIRTUAL      SerialUSB
#define SERIAL_PORT_HARDWARE_OPEN   Serial1
//#define SERIAL_PORT_HARDWARE_OPEN1  Serial2
//#define SERIAL_PORT_HARDWARE_OPEN2  Serial3
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE1       Serial1
//#define SERIAL_PORT_HARDWARE2       Serial2
//#define SERIAL_PORT_HARDWARE3       Serial3

#endif // __cplusplus


#ifdef __cplusplus
extern "C" {
#endif

#define DBG_PRINTF(...) DBG_vPrintf(DBG_VPRINTF_ENABLE, __VA_ARGS__)
#ifdef DBG_ENABLE
#define DBG_VPRINTF_ENABLE true
#else
#define DBG_VPRINTF_ENABLE false
#endif

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 1

#define SPI_INTERFACE        SPI0
#define SPI_INTERFACE_ID     ID_SPI0
#define SPI_CHANNELS_NUM 4
#define PIN_SPI_SS0          (19u)
#define PIN_SPI_SS1          (0u)
#define PIN_SPI_SS2          (1u)
#define BOARD_SPI_SS0        PIN_SPI_SS0
#define BOARD_SPI_SS1        PIN_SPI_SS1
#define BOARD_SPI_SS2        PIN_SPI_SS2
#define BOARD_SPI_DEFAULT_SS 0xFF

#define BOARD_PIN_TO_SPI_CHANNEL(x) (x==19 ? 0 : (x==0 ? 1 : (x==1 ? 2 : (x==0xFF ? 5 : 0) ) ) )

#define SPI0 NULL
#define ID_SPI0 -1

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1
#define WIRE_INTERFACE       0

#define VARIANT_MCK                     84000000


#define SYSTEM_WAKETIMER E_AHI_WAKE_TIMER_1
#if SYSTEM_WAKETIMER == E_AHI_WAKE_TIMER_0
#define SYSTEM_WAKETIMER_MASK E_AHI_WAKE_TIMER_MASK_0
#else
#define SYSTEM_WAKETIMER_MASK E_AHI_WAKE_TIMER_MASK_1
#endif

#define WTCOUNT2MSEC(cnt) ( (cnt) / 320000.f / (double)wakeTimerCalibrationValue() )
#define MSEC2WTCOUNT(ms)  ( (ms)  * 320000.f / (double)wakeTimerCalibrationValue() )

#define POWER_STATUS_WAKED		(0x1 << 0)
#define POWER_STATUS_RAM_RETAINING	(0x1 << 1)
#define POWER_STATUS_ANALOG_ON		(0x1 << 2)
#define POWER_STATUS_PROTOCOL_ON	(0x1 << 3)
#define POWER_STATUS_NONE_4		(0x1 << 4)
#define POWER_STATUS_NONE_5		(0x1 << 5)
#define POWER_STATUS_NONE_6		(0x1 << 6)
#define POWER_STATUS_WATCHDOG_CAUSED	(0x1 << 7)
#define POWER_STATUS_NONE_8		(0x1 << 8)
#define POWER_STATUS_NONE_9		(0x1 << 9)
#define POWER_STATUS_32KHZ_READY	(0x1 << 10)
#define POWER_STATUS_WAKED_FROM_DEEP	(0x1 << 11)
#define POWER_STATUS_NONE_12		(0x1 << 12)
#define POWER_STATUS_NONE_13		(0x1 << 13)
#define POWER_STATUS_NONE_14		(0x1 << 14)
#define POWER_STATUS_NONE_15		(0x1 << 15)

#define PIN_ADC1 0xA1
#define PIN_ADC2 0xA2
#define PIN_ADC3 0
#define PIN_ADC4 1

//static const uint8_t ADC1   = PIN_ADC1;
//static const uint8_t ADC2   = PIN_ADC2;
//static const uint8_t ADC3   = PIN_ADC3;
//static const uint8_t ADC4   = PIN_ADC4;

static const uint8_t A0   = PIN_ADC1;
static const uint8_t A1   = PIN_ADC2;
static const uint8_t A2   = PIN_ADC3;
static const uint8_t A3   = PIN_ADC4;

static const uint8_t PWM1 = 5;
static const uint8_t PWM2 = 0xD0;
static const uint8_t PWM3 = 0xD1;
static const uint8_t PWM4 = 8;

int UART0_RxHandler(uint8_t c);
int UART1_RxHandler(uint8_t c);

bool warmBoot();
bool waked();

extern uint32_t wakeTimerCalibrationValue();
extern void setWakeTimerCalibrationValue(uint32_t value);

#define ARDUINO_LOOP loop_

#define PUBLIC_PROCESS_THREAD(name, ev, data) 				\
PT_THREAD(process_thread_##name(struct pt *process_pt,	\
				       process_event_t ev,	\
				       process_data_t data))

#define LOOP() PUBLIC_PROCESS_THREAD(loop_, ev, data)

#ifdef __cplusplus
}
#endif

#endif /* _VARIANT_TWELITE_ */
