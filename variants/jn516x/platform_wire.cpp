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

#include "platform_wire.h"
#include <AppHardwareApi.h>
#define SI_CLOCK2PRESCALER(clk) ( (16L/((clk)/1000000L)/5L)-1 )


typedef enum
{
	WIRE_WRITE_FLAG = 0x0ul,
	WIRE_READ_FLAG
} SercomWireReadWriteFlag;

typedef enum
{
	WIRE_MASTER_ACT_NO_ACTION = 0,
	WIRE_MASTER_ACT_REPEAT_START,
	WIRE_MASTER_ACT_READ,
	WIRE_MASTER_ACT_STOP
} SercomMasterCommandWire;

int i2c_start( uint8_t txAddress, int readflag )
{
	// transmit buffer (blocking)
	DBG_PRINTF("vAHI_SiMasterWriteSlaveAddr %x\n", txAddress);
	vAHI_SiMasterWriteSlaveAddr(txAddress, readflag);

	bool ret = bAHI_SiMasterSetCmdReg(E_AHI_SI_START_BIT, E_AHI_SI_NO_STOP_BIT,
			                  E_AHI_SI_NO_SLAVE_READ, E_AHI_SI_SLAVE_WRITE,
					  E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
	(void)ret;//TODO
	
	while(bAHI_SiMasterPollTransferInProgress()); /* wait while busy */
	/* check to see if we get an ACK back*/ 
	if(bAHI_SiMasterCheckRxNack()) {
		return 0;
	}

	return 1;
}


void i2c_stop()
{
	vAHI_SiMasterSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_STOP_BIT,
				 E_AHI_SI_NO_SLAVE_READ, E_AHI_SI_NO_SLAVE_WRITE,
				 E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
}

int i2c_write_bytes(uint8_t* buffer, size_t length)
{
	// Send all buffer
	for(int i=0; i<length; i++) {
		vAHI_SiMasterWriteData8( buffer[i] );
		vAHI_SiMasterSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_NO_STOP_BIT,
				 E_AHI_SI_NO_SLAVE_READ, E_AHI_SI_SLAVE_WRITE,
				 E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
		
		while(bAHI_SiMasterPollTransferInProgress()); /* wait while busy */
		/* check to see if we get an ACK back*/ 
		if(bAHI_SiMasterCheckRxNack()) {
			return -1;
		}
	}
	return 0;
}

int i2c_write( uint8_t senddata )
{
	return i2c_write_bytes(&senddata, 1);
}

int i2c_read_bytes(uint8_t* buffer, size_t length) {
	int i=0;
	for (i=0; i<length; i++)
	{
		vAHI_SiMasterSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_NO_STOP_BIT,
					 E_AHI_SI_SLAVE_READ, E_AHI_SI_NO_SLAVE_WRITE,
					 i!=(length-1) ? E_AHI_SI_SEND_ACK : E_AHI_SI_SEND_NACK,
					 E_AHI_SI_NO_IRQ_ACK);
		while(bAHI_SiMasterPollTransferInProgress()); /* wait while busy */
		buffer[i] = u8AHI_SiMasterReadData8();          // Read data and send the ACK
	}
}

int i2c_read() {
	uint8_t byte = 0;
	i2c_read_bytes(&byte, 1);
	return byte;
}

void i2c_disable()
{
	vAHI_SiSlaveDisable();
	vAHI_SiMasterDisable();
}

int i2c_master_enable(uint32_t twiClock)
{
	vAHI_SiMasterConfigure(true, false, SI_CLOCK2PRESCALER(twiClock) ); // set to 100KHz
	return 0;
}

int i2c_slave_enable(uint8_t address)
{
	vAHI_SiSlaveConfigure(address, false, true, 0xFF, false);
	return 0;
}

int i2c_init()
{
	return 0;
}

struct Twi {
	int     (*init)();
	int	(*master_enable)(uint32_t);
	int	(*slave_enable)(uint8_t);
	void    (*disable)();
	int	(*start)(uint8_t, int);
	int	(*read_bytes)(uint8_t*, size_t);
	int	(*write_bytes)(uint8_t*, size_t);
	void	(*stop)();
};


struct Twi xtwi = {
	i2c_init,
	i2c_master_enable,
	i2c_slave_enable,
	i2c_disable,
	i2c_start,
	i2c_read_bytes,
	i2c_write_bytes,
	i2c_stop
};


TwoWire::TwoWire(Twi *_twi, void(*_beginCb)(void), void(*_endCb)(void)) :
	txAddress(0), twi(&xtwi), twiClock(TWI_CLOCK)
{
  twi->init();
}

void TwoWire::begin(void) {
  twi->master_enable(twiClock);
  master_mode = true;
}

void TwoWire::begin(uint8_t address) {
  twi->slave_enable(address);
  master_mode = false;
}

void TwoWire::setClock(uint32_t frequency) {
  twiClock = frequency;
  if(master_mode)
  {
    twi->disable();
    twi->master_enable(twiClock);
  }
}

void TwoWire::end() {
  twi->disable();
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

  if(twi->start(address, WIRE_READ_FLAG))
  {

    twi->read_bytes(rxBuffer._aucBuffer, quantity);
    rxBuffer._iHead = quantity;
    if (stopBit)
    {
      twi->stop();   // Send Stop
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
  if ( !twi->start( txAddress, WIRE_WRITE_FLAG ) )
  {
    twi->stop();
    return 2 ;  // Address error
  }

  // Send all buffer
  if(txBuffer.available() )
  {
    //txBuffer always start 0.
    if( twi->write_bytes(txBuffer._aucBuffer, txBuffer.available() ) )
    {
      twi->stop();
      return 3 ;  // Nack or error
    }
  }
  
  if (stopBit)
  {
    twi->stop();
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
extern "C" {
static void SiInterruptHandler(uint32 /*u32Device*/, uint32 u32ItemBitmap) {
	if(u32ItemBitmap & E_AHI_SIM_RXACK_MASK) {
	}
	if(u32ItemBitmap & E_AHI_SIM_BUSY_MASK) {
	}
	if(u32ItemBitmap & E_AHI_SIM_AL_MASK) {
	}
	if(u32ItemBitmap & E_AHI_SIM_ICMD_MASK) {
	}
	if(u32ItemBitmap & E_AHI_SIM_TIP_MASK) {
	}
	if(u32ItemBitmap & E_AHI_SIM_INT_STATUS_MASK) {
	}
	if(u32ItemBitmap & E_AHI_SIM_RXACK_MASK) {
	}
	if(u32ItemBitmap & E_AHI_SIM_RXACK_MASK) {
	}
	if(u32ItemBitmap & E_AHI_SIM_RXACK_MASK) {
	}
    /* Serial Interface Slave */
#define E_AHI_SIS_ERROR_MASK            (1 << 4)
#define E_AHI_SIS_LAST_DATA_MASK        (1 << 3)
#define E_AHI_SIS_DATA_WA_MASK          (1 << 2)
#define E_AHI_SIS_DATA_RTKN_MASK        (1 << 1)
#define E_AHI_SIS_DATA_RR_MASK          (1 << 0)
}
}
static void Wire_Init(void) {
	vAHI_SiRegisterCallback(SiInterruptHandler);
}

static void Wire_Deinit(void) {
	//TODO
}

TwoWire Wire = TwoWire(WIRE_INTERFACE, Wire_Init, Wire_Deinit);

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

