#include <jendefs.h>

#include "i2c-driver.h"
#include <AppHardwareApi.h>

#define SI_CLOCK2PRESCALER(clk) ( (16L/((clk)/1000000L)/5L)-1 )

int i2c_init()
{
	return 0;
}

int i2c_master_enable(uint32_t twiClock)
{
	vAHI_SiMasterConfigure(1, 0, SI_CLOCK2PRESCALER(twiClock) ); // set to 100KHz
	return 0;
}

int i2c_slave_enable(uint8_t address)
{
	vAHI_SiSlaveConfigure(address, 0, 1, 0xFF, 0);
	return 0;
}

void i2c_disable()
{
	vAHI_SiMasterDisable();
	vAHI_SiSlaveDisable();
}

int i2c_start( uint8_t txAddress, int readflag )
{
	// transmit buffer (blocking)
	//DBG_PRINTF("vAHI_SiMasterWriteSlaveAddr %x\n", txAddress);
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

int i2c_read_bytes(uint8_t* buffer, size_t length)
{
	unsigned int i=0;
	for (i=0; i<length; i++)
	{
		vAHI_SiMasterSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_NO_STOP_BIT,
					 E_AHI_SI_SLAVE_READ, E_AHI_SI_NO_SLAVE_WRITE,
					 i!=(length-1) ? E_AHI_SI_SEND_ACK : E_AHI_SI_SEND_NACK,
					 E_AHI_SI_NO_IRQ_ACK);
		while(bAHI_SiMasterPollTransferInProgress()); /* wait while busy */
		buffer[i] = u8AHI_SiMasterReadData8();          // Read data and send the ACK
	}
	return i;
}

int i2c_read()
{
	uint8_t byte = 0;
	i2c_read_bytes(&byte, 1);
	return byte;
}

int i2c_write_bytes(uint8_t* buffer, size_t length)
{
	// Send all buffer
	unsigned int i=0;
	for(i=0; i<length; i++) {
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

void i2c_stop()
{
	vAHI_SiMasterSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_STOP_BIT,
				 E_AHI_SI_NO_SLAVE_READ, E_AHI_SI_NO_SLAVE_WRITE,
				 E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
}


