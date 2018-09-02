/*
  Copyright (c) 2017-2018 Tokita, Hiroshi  All right reserved.

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

#ifndef I2CDRIVER_H__
#define I2CDRIVER_H__

#if defined __cplusplus
extern "C" {
#endif

#include <jendefs.h>
#include "contiki-conf.h"
#include <ringbuf.h>

#define JN516x_I2C_BUFFERSIZE 64

struct jn516x_i2c_devinfo
{
  bool req_received;
  bool data_received;

  struct ringbuf txbuf;
  uint8_t tbuf[JN516x_I2C_BUFFERSIZE];
  struct ringbuf rxbuf;
  uint8_t rbuf[JN516x_I2C_BUFFERSIZE];
};

extern struct jn516x_i2c_devinfo jn516x_i2c_info;

int jn516x_i2c_init(void* dev);
int jn516x_i2c_master_enable(void* dev, uint32_t clock);
int jn516x_i2c_slave_enable(void* dev, uint8_t address);
void jn516x_i2c_disable(void* dev);

int jn516x_i2c_master_start(void* dev, uint8_t address, int readflag);
int jn516x_i2c_master_read(void* dev, size_t length);
int jn516x_i2c_master_write(void* dev);
int jn516x_i2c_slave_write(void* dev);
void jn516x_i2c_master_stop(void* dev);

int jn516x_i2c_rx_available(void* dev);
void jn516x_i2c_rx_clear(void* dev);
int jn516x_i2c_rx_read(void* dev);
int jn516x_i2c_rx_peek(void* dev);
int jn516x_i2c_tx_available(void* dev);
void jn516x_i2c_tx_clear(void* dev);
int jn516x_i2c_tx_put(void* dev, uint8_t data);
int jn516x_i2c_tx_full(void* dev);

int jn516x_i2c_request_received(void* dev);
int jn516x_i2c_data_received(void* dev);

#if defined __cplusplus
}
#endif

#endif //I2C_DRIVER_H__

