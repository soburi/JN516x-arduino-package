#ifndef I2CDRIVER_H__
#define I2CDRIVER_H__

#if defined __cplusplus
extern "C" {
#endif

#include <jendefs.h>
#include "contiki-conf.h"

int i2c_master_enable(uint32_t clock);
int i2c_slave_enable(uint8_t address);
void i2c_disable();

int i2c_start( uint8_t address, int readflag );
int i2c_write_bytes(uint8_t* buffer, size_t length);
int i2c_write( uint8_t senddata );
int i2c_read_bytes(uint8_t* buffer, size_t length);
int i2c_read();
void i2c_stop();

#if defined __cplusplus
}
#endif

#endif //I2C_DRIVER_H__

