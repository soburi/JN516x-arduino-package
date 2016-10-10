/*
 * SPI Master library for Arduino Zero.
 * Copyright (c) 2015 Arduino LLC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <Arduino.h>
#include <linux/spi/spidev.h>

class SPIDevFile {
  public:
    SPIDevFile(const char* path_, uint32_t pin_) : path(path_), pin(pin_) {}
    const char* path;
    uint32_t pin;
};

// SPI_HAS_TRANSACTION means SPI has
//   - beginTransaction()
//   - endTransaction()
//   - usingInterrupt()
//   - SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

#define SPI_MODE0 SPI_MODE_0 
#define SPI_MODE1 SPI_MODE_1
#define SPI_MODE2 SPI_MODE_2
#define SPI_MODE3 SPI_MODE_3

class SPISettings {
  public:
  SPISettings(uint32_t clock, BitOrder bitOrder, uint8_t dataMode) : clock(0), mode(0) {
    //if (__builtin_constant_p(clock)) {
      init_AlwaysInline(clock, bitOrder, dataMode);
    //} else {
    //  init_MightInline(clock, bitOrder, dataMode);
    //}
  }

  // Default speed set to 4MHz, SPI mode set to MODE 0 and Bit order set to MSB first.
  SPISettings() { init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0); }

  private:
  void init_MightInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode) {
    init_AlwaysInline(clock, bitOrder, dataMode);
  }

  void init_AlwaysInline(uint32_t clock, BitOrder bitOrder, uint8_t dataMode) __attribute__((__always_inline__)) {
    this->clock = clock;
    this->mode |= (bitOrder == LSBFIRST) ? SPI_LSB_FIRST : 0;
    this->mode |= dataMode;
  }
  uint32_t clock;
  BitOrder order;
  uint8_t  mode;

  friend class SPIClass;
};

class SPIClass {
  public:
  SPIClass(const SPIDevFile devfile[]);


  byte transfer(uint8_t data);
  uint16_t transfer16(uint16_t data);
  void transfer(void *buf, size_t count);

  // Transaction Functions
  void usingInterrupt(int interruptNumber);
  void beginTransaction(SPISettings settings);
  void endTransaction(void);

  // SPI Configuration methods
  void attachInterrupt();
  void detachInterrupt();

  void begin();
  void end();

  void setBitOrder(BitOrder order);
  void setDataMode(uint8_t uc_mode);
  void setClockDivider(uint8_t uc_div);

  private:
  void init();
  void config(SPISettings settings);

  struct spi_ioc_transfer spi;
  BitOrder bitOrder;
  uint32_t clock;
  uint32_t mode;

  int fd;
  const SPIDevFile* spiDev;
  bool initialized;
};

extern SPIClass SPI;

// For compatibility with sketches designed for AVR @ 16 MHz
// New programs should use SPI.beginTransaction to set the SPI clock
#define SPI_CLOCK_DIV2   2   //  7.8MHz
#define SPI_CLOCK_DIV4   4   //  3.9MHz
#define SPI_CLOCK_DIV8   8   // 1953kHz
#define SPI_CLOCK_DIV16  16  //  976kHz
#define SPI_CLOCK_DIV32  32  //  488kHz
#define SPI_CLOCK_DIV64  64  //  244kHz
#define SPI_CLOCK_DIV128 128 //  122kHz

#endif
