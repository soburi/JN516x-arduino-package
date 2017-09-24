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

#include "SPI.h"
#include "variant.h"

#ifndef SPI_CLOCK
#define SPI_CLOCK SystemCoreClock
#endif

const SPISettings DEFAULT_SPI_SETTINGS = SPISettings();

SPIClass::SPIClass(struct spi_device* _spi)
         : spi(_spi)
         , divider(SPI_CLOCK/DEFAULT_SPI_SETTINGS.clock)
         , bitOrder(DEFAULT_SPI_SETTINGS.bitOrder)
         , dataMode(DEFAULT_SPI_SETTINGS.dataMode)
         , initialized(false)
{
  DBG_PRINTF("SPIClass::SPIClass\r\n");
}

void SPIClass::begin()
{
  init();

  divider = SPI_CLOCK/DEFAULT_SPI_SETTINGS.clock;
  bitOrder = DEFAULT_SPI_SETTINGS.bitOrder;
  dataMode = DEFAULT_SPI_SETTINGS.dataMode;

  config(DEFAULT_SPI_SETTINGS);
}

void SPIClass::init()
{
  DBG_PRINTF("SystemCoreClock %d\r\n", SystemCoreClock);
  DBG_PRINTF("DEFAULT_SPI_SETTINGS.clock %d\r\n", DEFAULT_SPI_SETTINGS.clock);
  if (initialized)
    return;

  spi->init(spi->devinfo);
  initialized = true;
}

void SPIClass::config(SPISettings settings)
{
  spi->configure(spi->devinfo,
                 settings.bitOrder == MSBFIRST ? 1 : 0,
                 settings.dataMode & 0x1 ? 1 : 0,
                 settings.dataMode & 0x2 ? 0 : 1,
                 settings.clock);
}

void SPIClass::end()
{
  spi->deinit(spi->devinfo);
  initialized = false;
}

#if 0
#ifndef interruptsStatus
#define interruptsStatus() __interruptsStatus()
static inline unsigned char __interruptsStatus(void) __attribute__((always_inline, unused));
static inline unsigned char __interruptsStatus(void)
{
  // See http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0497a/CHDBIBGJ.html
  return (__get_PRIMASK() ? 0 : 1);
}
#endif
#endif

void SPIClass::usingInterrupt(int interruptNumber)
{
  spi->mask_interrupt_on_transaction(spi->devinfo, interruptNumber);
}

void SPIClass::beginTransaction(SPISettings settings)
{
  config(settings);
  spi->start(spi->devinfo);
}

void SPIClass::endTransaction(void)
{
  spi->stop(spi->devinfo);
}

void SPIClass::setBitOrder(BitOrder order)
{
  bitOrder = order;
  if(initialized)
    config(SPISettings(SPI_CLOCK/divider, bitOrder, dataMode) );
}

void SPIClass::setDataMode(uint8_t mode)
{
  dataMode = mode;
  if(initialized)
    config(SPISettings(SPI_CLOCK/divider, bitOrder, dataMode) );
}

void SPIClass::setClockDivider(uint8_t div)
{
  divider = div;
  if(initialized)
    config(SPISettings(SPI_CLOCK/divider, bitOrder, dataMode) );
}

byte SPIClass::transfer(uint8_t data)
{
  return spi->transfer(spi->devinfo, data);
}

uint16_t SPIClass::transfer16(uint16_t data) {
  union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } t;

  t.val = data;

  if (bitOrder == LSBFIRST) {
    t.lsb = transfer(t.lsb);
    t.msb = transfer(t.msb);
  } else {
    t.msb = transfer(t.msb);
    t.lsb = transfer(t.lsb);
  }

  return t.val;
}

void SPIClass::transfer(void *buf, size_t count)
{
  uint8_t *buffer = reinterpret_cast<uint8_t *>(buf);
  for (size_t i=0; i<count; i++) {
    *buffer = transfer(*buffer);
    buffer++;
  }
}

void SPIClass::attachInterrupt() {
  // Should be enableInterrupt()
}

void SPIClass::detachInterrupt() {
  // Should be disableInterrupt()
}

#if SPI_INTERFACES_COUNT > 0
  SPIClass SPI(SPI_INTERFACE);
#endif
#if SPI_INTERFACES_COUNT > 1
  SPIClass SPI1(SPI1_INTERFACE);
#endif
#if SPI_INTERFACES_COUNT > 2
  SPIClass SPI2(SPI2_INTERFACE);
#endif
#if SPI_INTERFACES_COUNT > 3
  SPIClass SPI2(SPI2_INTERFACE);
#endif
#if SPI_INTERFACES_COUNT > 4
  SPIClass SPI3(SPI3_INTERFACE);
#endif
#if SPI_INTERFACES_COUNT > 5
  SPIClass SPI4(SPI4_INTERFACE);
#endif

