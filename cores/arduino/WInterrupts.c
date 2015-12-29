#include <contiki.h>
#include <lib/sensors.h>
#include <sys/process.h>
#include <dev/button-sensor.h>

#include "Arduino.h"

#include "wiring_private.h"


PROCESS(buttons_callback, "Buttons callback");

static void winterrupts_c_constructor() __attribute__((constructor));
static void winterrupts_c_constructor()
{
	static start_process_t button_l;
	button_l.process = &buttons_callback;
	add_start_process(&button_l);
}

PROCESS_THREAD(buttons_callback, ev, data)
{
	PROCESS_BEGIN();

	while(true)
	{
		PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event);

		struct sensors_sensor* button = (struct sensors_sensor*)data;
		int value = button->value(0);

		const struct buttonCallback* cb = button2ButtonCallback(button);

		if(cb)
		{
			if(  (cb->mode == CHANGE)
			  || (cb->mode == RISING  && value == 1)
			  || (cb->mode == FALLING && value == 0) )
			{
				cb->callback();
			}
		}
	}

	PROCESS_END();
}

void _attachInterruptDefault(uint32_t pin, voidFuncPtr callback, uint32_t mode)
{
	struct sensors_sensor* button = NULL;
	struct buttonCallback* cb = NULL;
	
	button = gpioPin2Button(pin);

	if(!button) return;

	cb = button2ButtonCallback(button);

	if(!cb) return;

	cb->callback = callback;
	cb->mode = mode;
	SENSORS_ACTIVATE(*button);
}

void _detachInterruptDefault(uint32_t pin)
{
	struct sensors_sensor* button = gpioPin2Button(pin);
	if(!button) return;
	SENSORS_DEACTIVATE(*button);
}

struct sensors_sensor* _gpioPin2ButtonDefault(uint32_t pin)
{
	return (struct sensors_sensor*)&button_sensor;
}

struct buttonCallback buttonCB = {0};
struct buttonCallback* _button2ButtonCallbackDefault(const struct sensors_sensor* button)
{
	return &buttonCB;
}

void attachInterrupt(uint32_t pin, voidFuncPtr callback, uint32_t mode) __attribute__((weak,alias("_attachInterruptDefault")));
void detachInterrupt(uint32_t pin) __attribute__((weak,alias("_detachInterruptDefault")));

struct sensors_sensor* gpioPin2Button(uint32_t pin) __attribute__((weak,alias("_gpioPin2ButtonDefault")));
struct buttonCallback* button2ButtonCallback(struct sensors_sensor*) __attribute__((weak,alias("_button2ButtonCallbackDefault")));
