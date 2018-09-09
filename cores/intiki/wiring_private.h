/*
  Copyright (c) 2014-2018 Tokita, Hiroshi  All right reserved.

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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "platform.h"
#include <sys/mt.h>
#include <sys/process.h>
#include <lib/sensors.h>
#include <lib/list.h>
#include <dev/button-sensor.h>

#include "WInterrupts.h"
#include "wiring_constants.h"

typedef int (*fp_condition)(process_event_t, process_data_t, void*);
typedef void (*fp_run)(void*);

enum main_thread_wait_type
{
	WT_NONE,
	WT_WAIT_UNTIL,
	WT_YIELD_UNTIL,
	WT_YIELD,
	WT_PAUSE
};

extern void yield_to_main_thread(enum main_thread_wait_type type, fp_run run, void* run_param, fp_condition condition, void* condition_param);
extern void yield_until(fp_run run, void* run_param, fp_condition condition, void* condition_param);

extern void yield_continue(fp_run run, void* run_param);

extern void post_continue();

typedef struct
{
	struct start_process_list_t* next;
	struct process* process;
} start_process_t;

extern void add_start_process(start_process_t* p);

struct buttonCallback
{
	const struct sensors_sensor* button;
	voidFuncPtr callback;
	uint32_t mode;
};
struct buttonCallback* button2ButtonCallback(struct sensors_sensor*);

struct sensors_sensor* gpioPin2Button(uint32_t pin);


struct uart_device {
	int     (*input)( uint8_t c);
	void    (*init)(void*, uint32_t, uint8_t, uint8_t, uint8_t, uint8_t);
	void    (*set_input)(void*, int (*input)(unsigned char));
	void    (*writeb)(void*, unsigned char);
	uint8_t (*busy)(void*);
	int	(*txbuffer_availables)(void*);
	void    (*deinit)(void*);
	void*	portinfo;
	uint8_t received;
};


struct i2c_device {
	int     (*init)(void*);
	int	(*master_enable)(void*, uint32_t);
	int	(*slave_enable)(void*, uint8_t);
	void    (*disable)(void*);
	int	(*master_start)(void*, uint8_t, int);
	int	(*master_read)(void*, size_t);
	int	(*master_write)(void*);
	void	(*master_stop)(void*);
	int	(*slave_write)(void*);
	int	(*rx_available)(void*);
	void	(*rx_clear)(void*);
	int	(*rx_read)(void*);
	int	(*rx_peek)(void*);
	int	(*tx_available)(void*);
	void	(*tx_clear)(void*);
	int	(*tx_put)(void*, uint8_t);
	int	(*tx_full)(void*);
	int	(*request_received)(void*);
	int	(*data_received)(void*);
	void*	devinfo;
};

struct spi_device {
	int     (*init)(void*);
	int	(*configure)(void*, bool, bool, bool, uint32_t);
	int	(*start)(void*);
	void	(*stop)(void*);
	uint8_t (*transfer)(void*, uint8_t);
	int	(*mask_interrupt_on_transaction)(void*, int);
	int     (*deinit)(void*);
	void*	devinfo;
};

struct power_device {
	void (*idle)(void*, uint32_t);
	void (*sleep)(void*, uint32_t);
	void (*deepsleep)(void*, uint32_t);
	uint32_t (*wakeup_reason)(void*);
	void* devinfo;
};

#ifdef __cplusplus
} // extern "C"
#endif
