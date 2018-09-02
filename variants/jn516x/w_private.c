/*
  Copyright (c) 2015-2018 Tokita, Hiroshi  All right reserved.

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

void sysctrl_callback(uint32_t u32Device, uint32_t u32ItemBitmap)
{
	DBG_PRINTF("sysctrl_callback(%x, %x)\n", u32Device, u32ItemBitmap);
	if(u32ItemBitmap & ALL_DIO_INT_MASK) {
		DIO_interrupt_handler(u32Device, u32ItemBitmap);
	}
}
