/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

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

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

extern "C" int arduino_main();

static int process_loop();
static void arduino_loop(void* none);
static void request_continue(void* data);
static int is_continue_event(process_event_t ev, process_data_t data, void* param);

struct main_thread_wait_t main_thread_wait;

static process_event_t immediately_continue;

LIST(start_processes);


extern struct process buttons_callback;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
PROCESS(main_thread, "main_thread");
PROCESS(echo_thread, "echo_thread");
#pragma GCC diagnostic pop

static void main_cpp_constructor() __attribute__((constructor(101)));
static void main_cpp_constructor()
{
	static start_process_t main_l;
	static start_process_t echo_l;
	main_l.process = &main_thread;
	echo_l.process = &echo_thread;
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

/*
 * \brief Main entry point of Arduino application
 */
int arduino_main()
{
	// Initialize watchdog
	watchdogSetup();

	init();

	initVariant();

	//TODO spin-loop delay(1);

#if defined(USBCON)
	USBDevice.init();
	USBDevice.attach();
#endif

	setup();

	start_process_list();

	return process_loop();
}

int process_loop()
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
		//yield and immidiatly return.
		main_thread_wait.type = WT_YIELD_UNTIL;
		main_thread_wait.run_param = NULL;
		main_thread_wait.run = request_continue;
		main_thread_wait.condition = is_continue_event;
		mt_yield();
	}
	mt_exit();
}

PROCESS_THREAD(main_thread, ev, data)
{
	static struct mt_thread arduino_thread;

	PROCESS_BEGIN();

	immediately_continue = process_alloc_event();

	mt_init();
	mt_start(&arduino_thread, arduino_loop, NULL);

	process_post(&echo_thread, immediately_continue, NULL);
	PROCESS_WAIT_EVENT_UNTIL(ev == immediately_continue);

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

PROCESS_THREAD(echo_thread, ev, data)
{
	PROCESS_BEGIN();
	while(1)
	{
		PROCESS_WAIT_EVENT();
		if(ev == immediately_continue)
		{
			process_post(&main_thread, immediately_continue, NULL);
		}
	}
	PROCESS_END();
}


static void request_continue(void* data)
{
	process_post(&echo_thread, immediately_continue, NULL);
}

static int is_continue_event(process_event_t ev, process_data_t data, void* param)
{
	return (ev == immediately_continue);
}
