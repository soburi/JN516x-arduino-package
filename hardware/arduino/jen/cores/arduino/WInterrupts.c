#include "Arduino.h"

#include "wiring_private.h"

#include <AppHardwareApi.h>

#define DIO_NUM 20

typedef void (*interrupt_handler)(void);

static void nop() { }

static interrupt_handler handler_table[DIO_NUM] = {nop};

static void DIO_interrupt_handler(uint32_t device, uint32_t bits)
{
	DEBUG_STR("DIO_interrupt_handler ");
	DEBUG_HEX(bits);
	DEBUG_STR("\n");
	int i=0;
	uint32_t dio_bits = (bits & 0x1FFFFF);
	for(i=0; i<DIO_NUM; i++) {
		if( dio_bits& (0x1U<<i) ) {
			DEBUG_STR("call ");
			DEBUG_DEC(i);
			DEBUG_STR("\n");
			handler_table[i]();
		}
	}
	DEBUG_STR("end DIO_interrupt_handler \n");
}

void attachInterrupt(uint8_t pin, void (*callback)(void), int mode)
{
	if(!SysCtrl_DIO_interrupt_handler) {
		DEBUG_STR("set SysCtrl_DIO_interrupt_handler\n");
		SysCtrl_DIO_interrupt_handler = DIO_interrupt_handler;
	}

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

void detachInterrupt(uint8_t pin)
{
	if(pin > DIO_NUM) return;
	
	handler_table[pin] = nop;

	vAHI_DioInterruptEnable(0, (1UL<<pin));
}
