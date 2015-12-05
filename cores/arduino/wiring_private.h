#pragma once

#ifdef __cplusplus
extern "C"{
#endif

#include "platform.h"
#include <sys/mt.h>
#include <sys/process.h>
#include <lib/sensors.h>
#include <lib/list.h>
#include <dev/button-sensor.h>

#include "WInterrupts.h"
#include "wiring_constants.h"

#define DIO_NUM 20

extern uint32_t ticktimer_overflow_count;
extern voidFuncPtr handler_table[DIO_NUM];

extern void ticktimer_callback(uint32_t u32Device, uint32_t u32ItemBitmap);
extern void sysctrl_callback(uint32_t u32Device, uint32_t u32ItemBitmap);

typedef int (*fp_condition)(process_event_t, process_data_t, void*);
typedef void (*fp_run)(void*);

enum main_thread_wait_type {
	WT_NONE,
	WT_WAIT_UNTIL,
	WT_YIELD_UNTIL,
	WT_YIELD
};

struct main_thread_wait_t {
	enum main_thread_wait_type type;
	void* run_param;
	void* condition_param;
	fp_run run;
	fp_condition condition;
};

struct buttonCallback
{
	const struct sensors_sensor* button;
	voidFuncPtr callback;
	uint32_t mode;
};

extern struct main_thread_wait_t main_thread_wait;

typedef struct {
  struct start_process_list_t* next;
  struct process* process;
} start_process_t;

extern void add_start_process(start_process_t* p);

struct sensors_sensor* gpioPin2Button(uint32_t pin);
struct buttonCallback* button2ButtonCallback(struct sensors_sensor*);

#ifdef __cplusplus
} // extern "C"
#endif
