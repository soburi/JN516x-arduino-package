#include "WInterrupts.h"

#include "wiring_private.h"

#define DIO_NUM 20

typedef void (*interrupt_handler)(void);

static void nop() { }

static interrupt_handler handler_table[DIO_NUM] = {nop};

static void DIO_interrupt_handler(uint32_t device, uint32_t bits)
{
	DBG_PRINTF("DIO_interrupt_handler ");
	DBG_PRINTF("%x", bits);
	DBG_PRINTF("\n");
	int i=0;
	uint32_t dio_bits = (bits & 0x1FFFFF);
	for(i=0; i<DIO_NUM; i++) {
		if( dio_bits& (0x1U<<i) ) {
			DBG_PRINTF("call ");
			DBG_PRINTF("%d\r\n", i);
			DBG_PRINTF("\n");
			handler_table[i]();
		}
	}
	DBG_PRINTF("end DIO_interrupt_handler \n");
}


void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
	if(!SysCtrl_DIO_interrupt_handler) {
		DBG_PRINTF("set SysCtrl_DIO_interrupt_handler\n");
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

void detachInterrupt(uint32_t pin)
{
	if(pin > DIO_NUM) return;
	
	handler_table[pin] = nop;

	vAHI_DioInterruptEnable(0, (1UL<<pin));
}
