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
