#include <jendefs.h>
#include <stdbool.h>

#include "i2c-driver.h"
#include <AppHardwareApi.h>

#include "platform.h"

#include <sys/cc.h>
#include <ringbuf.h>

#define SI_CLOCK2PRESCALER(clk) ( (16L/((clk)/1000000L)/5L)-1 )

struct jn516x_i2c_devinfo jn516x_i2c_info;

extern void WIRE_IT_HANDLER();

static void master_callback(uint32_t u32Device, uint32_t u32ItemBitmap)
{
  (void)u32Device;
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
}

static void slave_callback(uint32_t u32Device, uint32_t u32ItemBitmap)
{
  (void)u32Device;
  if(u32ItemBitmap & E_AHI_SIS_DATA_RR_MASK) {
    jn516x_i2c_info.req_received = true;
    WIRE_IT_HANDLER();
    jn516x_i2c_info.req_received = false;
  }
  if(u32ItemBitmap & E_AHI_SIS_DATA_RTKN_MASK) {
    if(ringbuf_elements(&jn516x_i2c_info.txbuf) > 0) {
      vAHI_SiSlaveWriteData8(ringbuf_get(&jn516x_i2c_info.txbuf));
    }
  }
  if(u32ItemBitmap & E_AHI_SIS_DATA_WA_MASK) {
    ringbuf_put(&jn516x_i2c_info.rxbuf, u8AHI_SiSlaveReadData8() );
  }
  if(u32ItemBitmap & E_AHI_SIS_LAST_DATA_MASK) {
    jn516x_i2c_info.data_received = true;
    WIRE_IT_HANDLER();
    jn516x_i2c_info.data_received = false;
  }
  if(u32ItemBitmap & E_AHI_SIS_ERROR_MASK) {
    DBG_PRINTF("slave_callback(%lx, %lx)\n", u32Device, u32ItemBitmap);
  }

}

int jn516x_i2c_init(void* dev)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  ringbuf_init(&info->rxbuf, info->rbuf, JN516x_I2C_BUFFERSIZE);
  ringbuf_init(&info->txbuf, info->tbuf, JN516x_I2C_BUFFERSIZE);
  return 0;
}

int jn516x_i2c_master_enable(void* dev, uint32_t twiClock)
{
  (void)dev;
  vAHI_SiRegisterCallback(master_callback);
  vAHI_SiMasterConfigure(FALSE, TRUE, SI_CLOCK2PRESCALER(twiClock) ); // set to 100KHz
  return 0;
}

int jn516x_i2c_slave_enable(void* dev, uint8_t address)
{
  (void)dev;
  vAHI_SiRegisterCallback(slave_callback);
  DBG_PRINTF("vAHI_SiSlaveConfigure %d\n", address);
  vAHI_SiSlaveConfigure(address, FALSE, 0, 0x1F, FALSE);
  return 0;
}

void jn516x_i2c_disable(void* dev)
{
  (void)dev;
  vAHI_SiMasterDisable();
  vAHI_SiSlaveDisable();
}

int jn516x_i2c_master_start(void* dev, uint8_t txAddress, int readflag )
{
  (void)dev;
  // transmit buffer (blocking)
  //DBG_PRINTF("vAHI_SiMasterWriteSlaveAddr %x\n", txAddress);
  vAHI_SiMasterWriteSlaveAddr(txAddress, readflag);

  bool ret = bAHI_SiMasterSetCmdReg(E_AHI_SI_START_BIT, E_AHI_SI_NO_STOP_BIT,
			                  E_AHI_SI_NO_SLAVE_READ, E_AHI_SI_SLAVE_WRITE,
					  E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
  if(!ret) {
    return 0;
  }
	
  while(bAHI_SiMasterPollTransferInProgress()); /* wait while busy */
  /* check to see if we get an ACK back*/ 
  if(bAHI_SiMasterCheckRxNack()) {
    return 0;
  }

  return 1;
}

int jn516x_i2c_master_read(void* dev, size_t length)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  unsigned int i=0;
  for (i=0; i<length; i++) {
    vAHI_SiMasterSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_NO_STOP_BIT,
                           E_AHI_SI_SLAVE_READ, E_AHI_SI_NO_SLAVE_WRITE,
                           i!=(length-1) ? E_AHI_SI_SEND_ACK : E_AHI_SI_SEND_NACK,
                           E_AHI_SI_NO_IRQ_ACK);
    
    while(bAHI_SiMasterPollTransferInProgress()); /* wait while busy */
    uint8_t b = u8AHI_SiMasterReadData8();          // Read data and send the ACK
    ringbuf_put(&info->rxbuf, b);
  }
  return i;
}

int jn516x_i2c_master_write(void* dev)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  // Send all buffer
  while(ringbuf_elements(&info->txbuf) > 0) {
    vAHI_SiMasterWriteData8( ringbuf_get(&info->txbuf) );
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

void jn516x_i2c_master_stop(void* dev)
{
  (void)dev;
  vAHI_SiMasterSetCmdReg(E_AHI_SI_NO_START_BIT, E_AHI_SI_STOP_BIT,
                         E_AHI_SI_NO_SLAVE_READ, E_AHI_SI_NO_SLAVE_WRITE,
                         E_AHI_SI_SEND_ACK, E_AHI_SI_NO_IRQ_ACK);
  while(bAHI_SiMasterPollTransferInProgress()); /* wait while busy */
}

int jn516x_i2c_slave_write(void* dev)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  if(ringbuf_elements(&info->txbuf) > 0) {
    vAHI_SiSlaveWriteData8(ringbuf_get(&info->txbuf));
  }
  return 0;
}

int jn516x_i2c_rx_available(void* dev)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  return ringbuf_elements(&info->rxbuf);
}

void jn516x_i2c_rx_clear(void* dev)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  while(ringbuf_get(&info->rxbuf) != -1);
}

int jn516x_i2c_rx_read(void* dev)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  return ringbuf_get(&info->rxbuf);
}

int jn516x_i2c_rx_peek(void* dev)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  struct ringbuf *r = &info->rxbuf;
  if(((r->put_ptr - r->get_ptr) & r->mask) > 0) {
    uint8_t c = CC_ACCESS_NOW(uint8_t, r->data[r->get_ptr]);
    return c;
  } else {
    return -1;
  }
}

int jn516x_i2c_tx_available(void* dev)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  return ringbuf_elements(&info->txbuf);
}

void jn516x_i2c_tx_clear(void* dev)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  while(ringbuf_get(&info->txbuf) != -1);
}

int jn516x_i2c_tx_put(void* dev, uint8_t data)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  return ringbuf_put(&info->txbuf, data);
}

int jn516x_i2c_tx_full(void* dev)
{
  struct jn516x_i2c_devinfo* info = (struct jn516x_i2c_devinfo*)dev;
  return (ringbuf_elements(&info->txbuf) == (ringbuf_size(&info->txbuf)-1));
}

int jn516x_i2c_request_received(void* dev)
{
  return ((struct jn516x_i2c_devinfo*)dev)->req_received;
}

int jn516x_i2c_data_received(void* dev)
{
  return ((struct jn516x_i2c_devinfo*)dev)->data_received;
}
