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

#ifndef _VARIANT_JN516X_
#define _VARIANT_JN516X_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <dbg.h>
#include <dbg_uart.h>
#pragma GCC diagnostic pop

#include "Arduino.h"
#ifdef __cplusplus
#include "Uart.h"
#endif // __cplusplus

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define DBG_PRINTF(...) DBG_vPrintf(DBG_VPRINTF_ENABLE, __VA_ARGS__)
#ifdef DBG_ENABLE
#define DBG_VPRINTF_ENABLE true
#else
#define DBG_VPRINTF_ENABLE false
#endif
/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

/*
 * digitalPinToTimer(..) is AVR-specific and is not defined for SAMD
 * architecture. If you need to check if a pin supports PWM you must
 * use digitalPinHasPWM(..).
 *
 * https://github.com/arduino/Arduino/issues/1833
 */
// #define digitalPinToTimer(P)

// LEDs
#define PIN_LED2             (2)
#define PIN_LED3             (3)
#define LED_BUILTIN          PIN_LED2

/*
 * Analog pins
 */
#define PIN_ADC1 (22)
#define PIN_ADC2 (23)
#define PIN_ADC3 (0)
#define PIN_ADC4 (1)

static const uint8_t ADC1   = PIN_ADC1;
static const uint8_t ADC2   = PIN_ADC2;
static const uint8_t ADC3   = PIN_ADC3;
static const uint8_t ADC4   = PIN_ADC4;

static const uint8_t A0   = PIN_ADC1;
static const uint8_t A1   = PIN_ADC2;
static const uint8_t A2   = PIN_ADC3;
static const uint8_t A3   = PIN_ADC4;

#define PIN_D0 (20)
#define PIN_D1 (21)

static const uint8_t D0   = PIN_D0;
static const uint8_t D1   = PIN_D1;

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


extern struct i2c_device i2c0;
#define WIRE_INTERFACE       (&i2c0)

#define VARIANT_MCK                     84000000


#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus

/*	=========================
 *	===== SERCOM DEFINITION
 *	=========================
*/
#define HAS_UART_0
#define HAS_UART_1

extern Uart& Serial;
#ifdef HAS_UART_0
extern Uart Serial0;
#endif
#ifdef HAS_UART_1
extern Uart Serial1;
#endif
#ifdef HAS_UART_2
extern Uart Serial2;
#endif
#ifdef HAS_UART_3
extern Uart Serial3;
#endif

#endif

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
//#define SERIAL_PORT_USBVIRTUAL      SerialUSB
#define SERIAL_PORT_MONITOR         Serial
#define SERIAL_PORT_HARDWARE_OPEN   Serial1
//#define SERIAL_PORT_HARDWARE_OPEN1  Serial2
//#define SERIAL_PORT_HARDWARE_OPEN2  Serial3
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE1       Serial1
//#define SERIAL_PORT_HARDWARE2       Serial2
//#define SERIAL_PORT_HARDWARE3       Serial3

#endif /* _VARIANT_JN516X_ */

