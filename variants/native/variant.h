/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.

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

#ifndef _VARIANT_NATIVE_
#define _VARIANT_NATIVE_

// The definitions here needs a SAMD core >=1.6.6
#define ARDUINO_SAMD_VARIANT_COMPLIANCE 10606

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		(32768ul)

/** Master clock frequency */
#define VARIANT_MCK			  (48000000ul)

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
#include "Uart.h"
#endif

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// Number of pins defined in PinDescription array
#define PINS_COUNT           (54u)
#define NUM_DIGITAL_PINS     (20u)
#define NUM_ANALOG_INPUTS    (0u)
#define NUM_ANALOG_OUTPUTS   (2u)

// LEDs
#define PIN_LED_ACT          (27u)
#define PIN_LED              PIN_LED_ACT
#define LED_BUILTIN          PIN_LED_ACT

/*
 * Analog pins
 * NOTE: RasPi has no analog inputs.
 */
#define PIN_A0   (0xFF)
#define PIN_A1   (0xFF)
#define PIN_A2   (0xFF)
#define PIN_A3   (0xFF)
#define PIN_A4   (0xFF)
#define PIN_A5   (0xFF)
#define PIN_A6   (0xFF)
#define PIN_A7   (0xFF)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;
static const uint8_t A6 = PIN_A6;
static const uint8_t A7 = PIN_A7;

// Other pins
#define PIN_PWM0             (18)
#define PIN_PWM1             (19)
static const uint8_t PWM0 = PIN_PWM0;
static const uint8_t PWM1 = PIN_PWM1;

/*
 * Serial interfaces
 */
// Serial
#define PIN_SERIAL_RX        (0xFF)
#define PIN_SERIAL_TX        (0xFF)

// Serial1
#define PIN_SERIAL1_RX       (15ul)
#define PIN_SERIAL1_TX       (14ul)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 1

#define PIN_SPI_MISO         (9)
#define PIN_SPI_MOSI         (10)
#define PIN_SPI_SCK          (11)
#define PIN_SPI_SS0          (8)
#define PIN_SPI_SS1          (7)
#define SPI_INTERFACE        SPI0
#define SPI_INTERFACE_ID     ID_SPI0
#define SPI_CHANNELS_NUM 2
#define BOARD_SPI_SS0        PIN_SPI_SS0
#define BOARD_SPI_SS1        PIN_SPI_SS1
#define BOARD_SPI_DEFAULT_SS 8

static const uint8_t SS   = BOARD_SPI_SS0 ;
static const uint8_t SS1  = BOARD_SPI_SS1 ;
static const uint8_t MOSI = PIN_SPI_MOSI ;
static const uint8_t MISO = PIN_SPI_MISO ;
static const uint8_t SCK  = PIN_SPI_SCK ;

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA         (2u)
#define PIN_WIRE_SCL         (3u)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;


#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus

extern Uart& Console;
extern Uart Stderr;
extern Uart Serial;
extern Uart Serial1;

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
// Serial has no physical pins broken out, so it's not listed as HARDWARE port
#define SERIAL_PORT_HARDWARE        Serial
#define SERIAL_PORT_HARDWARE_OPEN   Serial1

#define SERIAL_PORT_HARDWARE1       Serial1

#endif /* _VARIANT_ */
