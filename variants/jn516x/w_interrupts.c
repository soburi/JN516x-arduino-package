/*
  Copyright (c) TOKITA Hiroshi.  All right reserved.

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

#include "WInterrupts.h"

#include "wiring_private.h"

#include <AppHardwareApi.h>
#include "variant.h"

static void nop() { }

voidFuncPtr handler_table[DIO_NUM] = {nop};

void DIO_interrupt_handler(uint32_t device, uint32_t bits)
{
	(void)device;
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
