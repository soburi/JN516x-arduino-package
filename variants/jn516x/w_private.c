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

#include "wiring_private.h"
#include "variant.h"

#define ALL_DIO_INT_MASK ( \
	E_AHI_DIO0_INT  | E_AHI_DIO1_INT  | E_AHI_DIO2_INT  | E_AHI_DIO3_INT  | \
	E_AHI_DIO4_INT  | E_AHI_DIO5_INT  | E_AHI_DIO6_INT  | E_AHI_DIO7_INT  | \
	E_AHI_DIO8_INT  | E_AHI_DIO9_INT  | E_AHI_DIO10_INT | E_AHI_DIO11_INT | \
	E_AHI_DIO12_INT | E_AHI_DIO13_INT | E_AHI_DIO14_INT | E_AHI_DIO15_INT | \
	E_AHI_DIO16_INT | E_AHI_DIO17_INT | E_AHI_DIO18_INT | E_AHI_DIO19_INT )

uint8_t segmentLength;
uint16_t segmentNumber;

uint32_t ticktimer_overflow_count = 0;

void ticktimer_callback(uint32 u32Device, uint32 u32ItemBitmap)
{
	(void)u32Device; (void)u32ItemBitmap;
	ticktimer_overflow_count++;
}


static void nop() { }

voidFuncPtr handler_table[DIO_NUM] = {nop};

static void DIO_interrupt_handler(uint32_t device, uint32_t bits)
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

void sysctrl_callback(uint32 u32Device, uint32 u32ItemBitmap)
{
	DBG_PRINTF("sysctrl_callback ");
	DBG_PRINTF("%x", u32Device);
	DBG_PRINTF(" ");
	DBG_PRINTF("%x", u32ItemBitmap);
	DBG_PRINTF("\n");
	if(u32ItemBitmap & ALL_DIO_INT_MASK) DIO_interrupt_handler(u32Device, u32ItemBitmap);
}
