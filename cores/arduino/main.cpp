/*
  Copyright (c) 2015 TOKITA Hiroshi.  All right reserved.

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

extern "C" {
#include <contiki.h>
#include <core/sys/process.h>
#include <core/sys/mt.h>
#include <core/lib/list.h>
}


#define ARDUINO_MAIN
#include "Arduino.h"

#include "wiring_private.h"

#include "watchdog.h"

void initVariant() __attribute__((weak));
void initVariant() { }

extern "C" int arduino_main();

static int process_loop();
static void arduino_loop(void* none);

static struct main_thread_wait_t
{
	enum main_thread_wait_type type;
	void* run_param;
	void* condition_param;
	fp_run run;
	fp_condition condition;
} main_thread_wait;

static void request_immediate_response(void* data);
static int is_immediate_response(process_event_t ev, process_data_t data, void* param);

static process_event_t immediate_response;

LIST(start_processes);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
PROCESS(main_thread, "main_thread");
PROCESS(immediate_response_thread, "immidiate_response_thread");
#pragma GCC diagnostic pop

static void main_cpp_constructor() __attribute__((constructor(101)));
static void main_cpp_constructor()
{
	static start_process_t main_l;
	static start_process_t echo_l;
	main_l.process = &main_thread;
	echo_l.process = &immediate_response_thread;
	add_start_process(&main_l);
	add_start_process(&echo_l);
}

void start_process_list()
{
	start_process_t* p;
	for( p = (start_process_t*)list_head(start_processes);
	     p != NULL;
	     p = (start_process_t*)list_item_next(p) )
	{
		process_start(p->process, NULL);
	}
}

void add_start_process(start_process_t* p)
{
	list_add(start_processes, p);
}

int arduino_main()
{
	// Initialize watchdog
	watchdogSetup();

	initVariant();

	setup();

	start_process_list();

	return process_loop();
}

static int process_loop()
{
	int r;
	while(1) {
		r = process_run();
	}
	return r;
}

static void arduino_loop(void* none)
{
	(void)none;

	for (;;)
	{
		loop();
		if (serialEventRun) serialEventRun();
		//yield and immediatly return.
		yield_until(request_immediate_response, NULL, is_immediate_response, NULL);
	}
	mt_exit();
}

PROCESS_THREAD(main_thread, ev, data)
{
	static struct mt_thread arduino_thread;

	PROCESS_BEGIN();

	immediate_response = process_alloc_event();

	mt_init();
	mt_start(&arduino_thread, arduino_loop, NULL);

	process_post(&immediate_response_thread, immediate_response, NULL);
	PROCESS_WAIT_EVENT_UNTIL(ev == immediate_response);

	while(1)
	{
		mt_exec(&arduino_thread);

		if(main_thread_wait.run)
		{
			main_thread_wait.run(main_thread_wait.run_param);
		}

		if(main_thread_wait.type == WT_WAIT_UNTIL && main_thread_wait.condition)
		{
			PROCESS_WAIT_UNTIL( main_thread_wait.condition(ev, data, main_thread_wait.condition_param) );
		}
		else if(main_thread_wait.type == WT_YIELD_UNTIL && main_thread_wait.condition)
		{
			PROCESS_YIELD_UNTIL( main_thread_wait.condition(ev, data, main_thread_wait.condition_param) );
		}
		else if(main_thread_wait.type == WT_YIELD)
		{
			PROCESS_YIELD();
		}
	}

	mt_stop(&arduino_thread);
	mt_remove();

	PROCESS_END();
}

PROCESS_THREAD(immediate_response_thread, ev, data)
{
	PROCESS_BEGIN();
	while(1)
	{
		PROCESS_WAIT_EVENT();
		if(ev == immediate_response)
		{
			process_post(&main_thread, immediate_response, NULL);
		}
	}
	PROCESS_END();
}

void yield_to_main_thread(enum main_thread_wait_type type, fp_run run, void* run_param, fp_condition condition, void* condition_param)
{
	main_thread_wait.type = type;
	main_thread_wait.run = run;
	main_thread_wait.run_param = run_param;
	main_thread_wait.condition = condition;
	main_thread_wait.condition_param = condition_param;
	mt_yield();
}

void yield_until(fp_run run, void* run_param, fp_condition condition, void* condition_param)
{
	yield_to_main_thread(WT_YIELD_UNTIL, run, run_param, condition, condition_param);
}

static void request_immediate_response(void* data)
{
	process_post(&immediate_response_thread, immediate_response, NULL);
}

static int is_immediate_response(process_event_t ev, process_data_t data, void* param)
{
	return (ev == immediate_response);
}
