/*
 * TWI/I2C library for Arduino Zero
 * Copyright (c) 2015 Arduino LLC. All rights reserved.
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

#include <cerrno>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include "platform.h"
#include "native_wire.h"

TwoWire::TwoWire(const char *s, uint8_t /*pinSDA*/, uint8_t /*pinSCL*/)
  : devfile(s)
{
}

void TwoWire::begin(void) {
  int ret = open(devfile, O_RDWR);
  ABORT_ON_ERROR(ret<0, "open(%s) failure:%d %s\n", devfile, errno, strerror(errno));

  this->fd = ret;
}

void TwoWire::begin(uint8_t /*address*/) {
  ABORT_ON_ERROR(1, "TwoWire::begin(uint8_t) I2C client mode is not supported.\n");
}

void TwoWire::setClock(uint32_t /*baudrate*/) {
}

void TwoWire::end() {
}

uint8_t TwoWire::requestFrom(uint8_t address, size_t quantity, bool /*stopBit*/)
{
  if(quantity == 0)
  {
    return 0;
  }

  rxBuffer.clear();

  LOG_DEBUG("ioctl(I2C_SLAVE) 0x%x\n", address);
  int byteRead = 0;
  int ret = ioctl(this->fd, I2C_SLAVE, address);
  if (ret < 0)
  {
    // error handling
  }
  else
  {
    char readbuf[SERIAL_BUFFER_SIZE] = {0};
    byteRead = ::read(this->fd, &readbuf, quantity);
    LOG_DEBUG("::read count=%d\n", byteRead);
    if(byteRead < 0) {
      // error handling
    }

    for(uint32_t i=0; i<quantity; i++)
    {
      rxBuffer.store_char(readbuf[i]);
    }
  }

  LOG_DEBUG("read count=%d\n", byteRead);

  return byteRead;
}

uint8_t TwoWire::requestFrom(uint8_t address, size_t quantity)
{
  return requestFrom(address, quantity, true);
}

void TwoWire::beginTransmission(uint8_t address) {

  // save address of target and clear buffer
  LOG_DEBUG("beginTransmission\n");
  txAddress = address;
  txBuffer.clear();

  transmissionBegun = true;

  LOG_DEBUG("ioctl(I2C_SLAVE) 0x%x\n", address);
  int ret = ioctl(this->fd, I2C_SLAVE, address);
  if (ret < 0)
  {
    // error handling
  }
  LOG_DEBUG("ioctl(I2C_SLAVE) ret=%d\n", ret);
}

// Errors:
//  0 : Success
//  1 : Data too long
//  2 : NACK on transmit of address
//  3 : NACK on transmit of data
//  4 : Other error
uint8_t TwoWire::endTransmission(bool /*stopBit*/)
{
  char writebuf[SERIAL_BUFFER_SIZE] = {0};
  size_t len = 0;

  // Send all buffer
  while( txBuffer.available() )
  {
    writebuf[len++] = txBuffer.read_char();
    LOG_DEBUG("read_char\n");
  }
  
  size_t byteWrite = ::write(this->fd, writebuf, len);
  LOG_DEBUG("::write count=%d\n", byteWrite);

  transmissionBegun = false;
  return byteWrite;
}

uint8_t TwoWire::endTransmission()
{
  return endTransmission(true);
}

size_t TwoWire::write(uint8_t ucData)
{
  // No writing, without begun transmission or a full buffer
  if ( !transmissionBegun || txBuffer.isFull() )
  {
    return 0 ;
  }

  txBuffer.store_char( ucData ) ;

  return 1 ;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
  //Try to store all data
  for(size_t i = 0; i < quantity; ++i)
  {
    //Return the number of data stored, when the buffer is full (if write return 0)
    if(!write(data[i]))
      return i;
  }

  //All data stored
  return quantity;
}

int TwoWire::available(void)
{
  return rxBuffer.available();
}

int TwoWire::read(void)
{
  return rxBuffer.read_char();
}

int TwoWire::peek(void)
{
  return rxBuffer.peek();
}

void TwoWire::flush(void)
{
  // Do nothing, use endTransmission(..) to force
  // data transfer.
}

void TwoWire::onReceive(void(*function)(int))
{
  onReceiveCallback = function;
}

void TwoWire::onRequest(void(*function)(void))
{
  onRequestCallback = function;
}

void TwoWire::onService(void)
{
}

#if WIRE_INTERFACES_COUNT > 0
  /* In case new variant doesn't define these macros,
   * we put here the ones for Arduino Zero.
   *
   * These values should be different on some variants!
   */
  //#ifndef PERIPH_WIRE
  //  #define PERIPH_WIRE          sercom3
  //  #define WIRE_IT_HANDLER      SERCOM3_Handler
  //#endif // PERIPH_WIRE
  TwoWire Wire("/dev/i2c-1", PIN_WIRE_SDA, PIN_WIRE_SCL);

  void WIRE_IT_HANDLER(void) {
    Wire.onService();
  }
#endif

#if WIRE_INTERFACES_COUNT > 1
  TwoWire Wire1(&PERIPH_WIRE1, PIN_WIRE1_SDA, PIN_WIRE1_SCL);

  void WIRE1_IT_HANDLER(void) {
    Wire1.onService();
  }
#endif

#if WIRE_INTERFACES_COUNT > 2
  TwoWire Wire2(&PERIPH_WIRE2, PIN_WIRE2_SDA, PIN_WIRE2_SCL);

  void WIRE2_IT_HANDLER(void) {
    Wire2.onService();
  }
#endif

#if WIRE_INTERFACES_COUNT > 3
  TwoWire Wire3(&PERIPH_WIRE3, PIN_WIRE3_SDA, PIN_WIRE3_SCL);

  void WIRE3_IT_HANDLER(void) {
    Wire3.onService();
  }
#endif

#if WIRE_INTERFACES_COUNT > 4
  TwoWire Wire4(&PERIPH_WIRE4, PIN_WIRE4_SDA, PIN_WIRE4_SCL);

  void WIRE4_IT_HANDLER(void) {
    Wire4.onService();
  }
#endif

#if WIRE_INTERFACES_COUNT > 5
  TwoWire Wire5(&PERIPH_WIRE5, PIN_WIRE5_SDA, PIN_WIRE5_SCL);

  void WIRE5_IT_HANDLER(void) {
    Wire5.onService();
  }
#endif

