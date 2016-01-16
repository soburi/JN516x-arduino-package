/*
 * TwoWire.h - TWI/I2C library for Arduino Due
 * Copyright (c) 2011 Cristian Maglie <c.maglie@arduino.cc>
 * All rights reserved.
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


TwoWire::TwoWire(Twi *_twi, void(*_beginCb)(void), void(*_endCb)(void)) :
	rxBufferIndex(0), rxBufferLength(0), txAddress(0),
		txBufferLength(0), srvBufferIndex(0), srvBufferLength(0),
			onBeginCallback(_beginCb), onEndCallback(_endCb),
				twi(_twi), status(UNINITIALIZED), twiClock(TWI_CLOCK)
{
}

void TwoWire::begin(void) {
	if (onBeginCallback)
		onBeginCallback();
	
	vAHI_SiSetLocation(false); 
	vAHI_SiMasterConfigure(true, false, SI_CLOCK2PRESCALER(twiClock) ); // set to 100KHz
	status = MASTER_IDLE;
}

void TwoWire::begin(uint8_t address) {
	if (onBeginCallback)
		onBeginCallback();

	vAHI_SiSlaveConfigure(address, false, true, 0xFF, false)	;
	status = SLAVE_IDLE;
}

void TwoWire::begin(int address) {
	begin((uint8_t) address);
}

void TwoWire::end(void) {
	//TODO

	if (onEndCallback)
		onEndCallback();
}

void TwoWire::setClock(uint32_t frequency) {
	twiClock = frequency;
	// Can't immidiately refrect.
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop) {
	if (quantity > BUFFER_LENGTH)
		quantity = BUFFER_LENGTH;

	DBG_PRINTF("vAHI_SiMasterWriteSlaveAddr %x\r\n", address);
	vAHI_SiMasterWriteSlaveAddr(address, true);
	vAHI_SiSetCmdReg(E_AHI_SI_START_BIT, E_AHI_SI_NO_STOP_BIT,
			 E_AHI_SI_NO_SLAVE_READ, E_AHI_SI_SLAVE_WRITE,
			 E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
	while(bAHI_SiPollTransferInProgress()); /* wait while busy */

	// perform blocking read into buffer
	int readed = 0;

	do {
		if(readed == (quantity-1) ) {
			vAHI_SiSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_STOP_BIT,
					 E_AHI_SI_SLAVE_READ, E_AHI_SI_NO_SLAVE_WRITE,
					 E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
		}
		else {
			vAHI_SiSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_NO_STOP_BIT,
					 E_AHI_SI_SLAVE_READ, E_AHI_SI_NO_SLAVE_WRITE,
					 E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
		}

		while(bAHI_SiPollTransferInProgress()); /* wait while busy */

		rxBuffer[readed++] = u8AHI_SiMasterReadData8();
		DBG_PRINTF("u8AHI_SiMasterReadData8 %x\r\n", rxBuffer[readed-1]);
	} while (readed < quantity);

	// set rx buffer iterator vars
	rxBufferIndex = 0;
	rxBufferLength = readed;

	return readed;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint32_t) 0, (uint8_t) 0, (uint8_t) sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) sendStop);
}

void TwoWire::beginTransmission(uint8_t address) {
	status = MASTER_SEND;

	// save address of target and empty buffer
	txAddress = address;
	txBufferLength = 0;
}

void TwoWire::beginTransmission(int address) {
	beginTransmission((uint8_t) address);
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to
//	perform a repeated start.
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop) {
	uint8_t error = 0;
	// transmit buffer (blocking)
	DBG_PRINTF("vAHI_SiMasterWriteSlaveAddr %x\r\n", txAddress);
	vAHI_SiMasterWriteSlaveAddr(txAddress, false);
	DBG_PRINTF("bAHI_SiMasterSetCmdReg\r\n");

	bool ret = bAHI_SiMasterSetCmdReg(E_AHI_SI_START_BIT, E_AHI_SI_NO_STOP_BIT,
			                  E_AHI_SI_NO_SLAVE_READ, E_AHI_SI_SLAVE_WRITE,
					  E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
	(void)ret;//TODO
	
	DBG_PRINTF("bAHI_SiPollTransferInProgress\r\n");
	while(bAHI_SiPollTransferInProgress()); /* wait while busy */
	/* check to see if we get an ACK back*/
	DBG_PRINTF("bAHI_SiPollRxNack\r\n");
	if(bAHI_SiPollRxNack())
		error = 2;	// error, got NACK on address transmit
	
	if (error == 0) {
		uint16_t sent = 0;
		while (sent < txBufferLength) {
			DBG_PRINTF("sent %d\n", txBuffer[sent]);
			vAHI_SiMasterWriteData8(txBuffer[sent++]);
			if(sent == (txBufferLength) ) {
				vAHI_SiSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_STOP_BIT,
						 E_AHI_SI_NO_SLAVE_READ, E_AHI_SI_SLAVE_WRITE,
						 E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
			}
			else {
				vAHI_SiSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_NO_STOP_BIT,
						 E_AHI_SI_NO_SLAVE_READ, E_AHI_SI_SLAVE_WRITE,
						 E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);

			}
			while(bAHI_SiPollTransferInProgress()); /* wait while busy */
			/* check to see if we get an ACK back*/
			if(bAHI_SiPollRxNack()) {
				error = 3;	// error, got NACK during data transmmit
				vAHI_SiSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_STOP_BIT,
						 E_AHI_SI_NO_SLAVE_READ, E_AHI_SI_NO_SLAVE_WRITE,
						 E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
			}

			DBG_PRINTF("sent done %d\n", sent);
		}
	}
	
	//if (error == 0) {
	//	TWI_Stop(twi);
	//	if (!TWI_WaitTransferComplete(twi, XMIT_TIMEOUT))
	//		error = 4;	// error, finishing up
	//}

	txBufferLength = 0;		// empty buffer
	status = MASTER_IDLE;
	return error;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
	return endTransmission(true);
}

size_t TwoWire::write(uint8_t data) {
	if (status == MASTER_SEND) {
		if (txBufferLength >= BUFFER_LENGTH)
			return 0;
		txBuffer[txBufferLength++] = data;
		return 1;
	} else {
		if (srvBufferLength >= BUFFER_LENGTH)
			return 0;
		srvBuffer[srvBufferLength++] = data;
		return 1;
	}
}

size_t TwoWire::write(const uint8_t *data, size_t quantity) {
	if (status == MASTER_SEND) {
		for (size_t i = 0; i < quantity; ++i) {
			if (txBufferLength >= BUFFER_LENGTH)
				return i;
			txBuffer[txBufferLength++] = data[i];
		}
	} else {
		for (size_t i = 0; i < quantity; ++i) {
			if (srvBufferLength >= BUFFER_LENGTH)
				return i;
			srvBuffer[srvBufferLength++] = data[i];
		}
	}
	return quantity;
}

int TwoWire::available(void) {
	return rxBufferLength - rxBufferIndex;
}

int TwoWire::read(void) {
	if (rxBufferIndex < rxBufferLength)
		return rxBuffer[rxBufferIndex++];
	return -1;
}

int TwoWire::peek(void) {
	if (rxBufferIndex < rxBufferLength)
		return rxBuffer[rxBufferIndex];
	return -1;
}

void TwoWire::flush(void) {
	// Do nothing, use endTransmission(..) to force
	// data transfer.
}

void TwoWire::onReceive(void(*function)(int)) {
	onReceiveCallback = function;
}

void TwoWire::onRequest(void(*function)(void)) {
	onRequestCallback = function;
}

void TwoWire::onService(void) {
}

#if WIRE_INTERFACES_COUNT > 0
extern "C" {
static void SiInterruptHandler(uint32 u32Device, uint32 u32ItemBitmap) {
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
static void Wire1_Init(void) {
	pmc_enable_periph_clk(WIRE1_INTERFACE_ID);
	PIO_Configure(
			g_APinDescription[PIN_WIRE1_SDA].pPort,
			g_APinDescription[PIN_WIRE1_SDA].ulPinType,
			g_APinDescription[PIN_WIRE1_SDA].ulPin,
			g_APinDescription[PIN_WIRE1_SDA].ulPinConfiguration);
	PIO_Configure(
			g_APinDescription[PIN_WIRE1_SCL].pPort,
			g_APinDescription[PIN_WIRE1_SCL].ulPinType,
			g_APinDescription[PIN_WIRE1_SCL].ulPin,
			g_APinDescription[PIN_WIRE1_SCL].ulPinConfiguration);

	NVIC_DisableIRQ(WIRE1_ISR_ID);
	NVIC_ClearPendingIRQ(WIRE1_ISR_ID);
	NVIC_SetPriority(WIRE1_ISR_ID, 0);
	NVIC_EnableIRQ(WIRE1_ISR_ID);
}

static void Wire1_Deinit(void) {
	NVIC_DisableIRQ(WIRE1_ISR_ID);
	NVIC_ClearPendingIRQ(WIRE1_ISR_ID);

	pmc_disable_periph_clk(WIRE1_INTERFACE_ID);

	// no need to undo PIO_Configure, 
	// as Peripheral A was enable by default before,
	// and pullups were not enabled
}

TwoWire Wire1 = TwoWire(WIRE1_INTERFACE, Wire1_Init, Wire1_Deinit);

void WIRE1_ISR_HANDLER(void) {
	Wire1.onService();
}
#endif
