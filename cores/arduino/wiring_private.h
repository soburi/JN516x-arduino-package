/*
  Copyright (c) 2011 Arduino.  All right reserved.

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

#ifndef WiringPrivate_h
#define WiringPrivate_h

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"{
#endif

#include "platform.h"

#include "wiring_constants.h"

#define DIO_NUM 20

extern uint32_t ticktimer_overflow_count;
typedef void (*void_callback_void)(void);
extern void_callback_void handler_table[DIO_NUM];

extern void ticktimer_callback(uint32_t u32Device, uint32_t u32ItemBitmap);
extern void sysctrl_callback(uint32_t u32Device, uint32_t u32ItemBitmap);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
