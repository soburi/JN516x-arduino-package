#include "WInterrupts.h"

#include "wiring_private.h"

#include <AppHardwareApi.h>

static void nop() { }


void attachInterrupt(uint32_t pin, voidFuncPtr callback, uint32_t mode)
{
	if(pin > DIO_NUM) return;
	
	handler_table[pin] = callback;

	vAHI_DioInterruptEnable((1UL<<pin), 0);
	if(mode == RISING || mode == LOW || mode == CHANGE || mode ==HIGH) {
		vAHI_DioInterruptEdge((1UL<<pin), 0);
	}
	else {
		vAHI_DioInterruptEdge(0, (1UL<<pin));
	}
}

void detachInterrupt(uint32_t pin)
{
	if(pin > DIO_NUM) return;
	
	handler_table[pin] = nop;

	vAHI_DioInterruptEnable(0, (1UL<<pin));
}
