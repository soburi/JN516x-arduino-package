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

extern "C" {
#include <string.h>
}

#include <Arduino.h>
#include <wiring_private.h>

#include "platform_wire.h"

enum
{
	WIRE_WRITE_FLAG = 0x0ul,
	WIRE_READ_FLAG
};

TwoWire::TwoWire(struct i2c_device*_twi) :
	i2c(_twi), clock(TWI_CLOCK)
{
  i2c->init(i2c->devinfo);
}

void TwoWire::begin(void) {
  i2c->master_enable(i2c->devinfo, clock);
  master_mode = true;
}

void TwoWire::begin(uint8_t address) {
  i2c->slave_enable(i2c->devinfo, address);
  master_mode = false;
}

void TwoWire::setClock(uint32_t frequency) {
  clock = frequency;
  if(master_mode)
  {
    i2c->disable(i2c->devinfo);
    i2c->master_enable(i2c->devinfo, clock);
  }
}

void TwoWire::end() {
  i2c->disable(i2c->devinfo);
}

uint8_t TwoWire::requestFrom(uint8_t address, size_t quantity, bool stopBit)
{
  if(quantity == 0)
  {
    return 0;
  }

  size_t byteRead = 0;

  rxBuffer.clear();

  // clamp to buffer length
  if(quantity > SERIAL_BUFFER_SIZE)
  {
    quantity = SERIAL_BUFFER_SIZE;
  }

  if(i2c->start(i2c->devinfo, address, WIRE_READ_FLAG))
  {

    i2c->read_bytes(i2c->devinfo, rxBuffer._aucBuffer, quantity);
    rxBuffer._iHead = quantity;

    if (stopBit)
    {
      i2c->stop(i2c->devinfo);   // Send Stop
    }
  }

  return byteRead;
}

uint8_t TwoWire::requestFrom(uint8_t address, size_t quantity)
{
  return requestFrom(address, quantity, true);
}

void TwoWire::beginTransmission(uint8_t address) {
  // save address of target and clear buffer
  txAddress = address;
  txBuffer.clear();

  transmissionBegun = true;
}

// Errors:
//  0 : Success
//  1 : Data too long
//  2 : NACK on transmit of address
//  3 : NACK on transmit of data
//  4 : Other error
uint8_t TwoWire::endTransmission(bool stopBit)
{
  transmissionBegun = false ;

  // Start I2C transmission
  if ( !i2c->start(i2c->devinfo, txAddress, WIRE_WRITE_FLAG ) )
  {
    i2c->stop(i2c->devinfo);
    return 2 ;  // Address error
  }

  // Send all buffer
  if(txBuffer.available() )
  {
    //txBuffer always start 0.
    if( i2c->write_bytes(i2c->devinfo, txBuffer._aucBuffer, txBuffer.available() ) )
    {
      i2c->stop(i2c->devinfo);
      return 3 ;  // Nack or error
    }
  }
  
  if (stopBit)
  {
    i2c->stop(i2c->devinfo);
  }   

  return 0;
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
    setWriteError();
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
  TwoWire Wire = TwoWire(WIRE_INTERFACE);

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

