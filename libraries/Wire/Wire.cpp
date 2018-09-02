/*
 * TWI/I2C library for Arduino Zero
 * Copyright (c) 2015 Arduino LLC. All rights reserved.
 * Copyright (c) 2015-2018 Tokita, Hiroshi
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
void WIRE_IT_HANDLER();
void WIRE1_IT_HANDLER();
void WIRE2_IT_HANDLER();
void WIRE3_IT_HANDLER();
void WIRE4_IT_HANDLER();
void WIRE5_IT_HANDLER();
}

#include <Arduino.h>
#include <wiring_private.h>

#include "Wire.h"

enum
{
  WIRE_WRITE_FLAG = 0x0ul,
  WIRE_READ_FLAG
};

static void nop_reqcb()
{
  DBG_PRINTF("nop_reqcb\n");
}
static void nop_recvcb(int x)
{
  (void)x;
  DBG_PRINTF("nop_reqcb %d\n", x);
}

TwoWire::TwoWire(struct i2c_device*_twi)
  : i2c(_twi)
  , clock(TWI_CLOCK)
  , onRequestCallback(nop_reqcb)
  , onReceiveCallback(nop_recvcb)
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
  transmissionBegun = true;
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

  i2c->rx_clear(i2c->devinfo);

  // clamp to buffer length
  if(quantity > SERIAL_BUFFER_SIZE)
  {
    quantity = SERIAL_BUFFER_SIZE;
  }

  if(i2c->master_start(i2c->devinfo, address, WIRE_READ_FLAG))
  {
    byteRead = i2c->master_read(i2c->devinfo, quantity);

    if (stopBit)
    {
      i2c->master_stop(i2c->devinfo);   // Send Stop
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
  i2c->tx_clear(i2c->devinfo);

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
  if ( !i2c->master_start(i2c->devinfo, txAddress, WIRE_WRITE_FLAG ) )
  {
    i2c->master_stop(i2c->devinfo);
    return 2 ;  // Address error
  }

  // Send all buffer
  if(i2c->tx_available(i2c->devinfo) )
  {
    //txBuffer always start 0.
    if( i2c->master_write(i2c->devinfo) )
    {
      i2c->master_stop(i2c->devinfo);
      return 3 ;  // Nack or error
    }
  }
  
  if (stopBit)
  {
    i2c->master_stop(i2c->devinfo);
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
  if ( !transmissionBegun || i2c->tx_full(i2c->devinfo) )
  {
    setWriteError();
    return 0 ;
  }

  i2c->tx_put(i2c->devinfo, ucData);
  
  if(master_mode == false)
  {
    i2c->slave_write(i2c->devinfo);
  }

  return 1 ;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
  //Try to store all data
  for(size_t i = 0; i < quantity; ++i)
  {
    i2c->tx_put(i2c->devinfo, data[i]);
  }

  if(master_mode == false)
  {
    i2c->slave_write(i2c->devinfo);
  }
  //All data stored
  return quantity;
}

int TwoWire::available(void)
{
  return i2c->rx_available(i2c->devinfo);
}

int TwoWire::read(void)
{
  return i2c->rx_read(i2c->devinfo);
}

int TwoWire::peek(void)
{
  return i2c->rx_peek(i2c->devinfo);
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
  if(i2c->request_received(i2c->devinfo))
  {
    onRequestCallback();
  }
  else if(i2c->data_received(i2c->devinfo))
  {
    onReceiveCallback(i2c->rx_available(i2c->devinfo));
  }
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

