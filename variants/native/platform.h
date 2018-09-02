/*
  Copyright (c) 2016 Tokita, Hiroshi  All right reserved.

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

#ifndef _VARIANT_PLATFORM_H
#define _VARIANT_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#undef F_CPU

#include <contiki.h>
#include <net/netstack.h>
#include <dev/watchdog.h>
#include <lib/random.h>

#ifdef __cplusplus
}
#endif

#define PLATFORM_SPI_HEADER "native_spi.h"
#define PLATFORM_WIRE_HEADER "native_wire.h"

#define DBG_PRINTF(...) { \
	char* dbg_printf_buf_ = (char*)malloc(1024); \
	sprintf(dbg_printf_buf_ , __VA_ARGS__); \
	fwrite(dbg_printf_buf_, 1, strnlen(dbg_printf_buf_, 1024), stderr); \
	free(dbg_printf_buf_); \
}

#ifndef LOGLEVEL
#define LOGLEVEL 4
#endif

#if LOGLEVEL <= 1
#define LOG_TRACE(...) DBG_PRINTF(__VA_ARGS__)
#else
#define LOG_TRACE(...)
#endif

#if LOGLEVEL <= 2
#define LOG_VERBOSE(...) DBG_PRINTF(__VA_ARGS__)
#else
#define LOG_VERBOSE(...)
#endif

#if LOGLEVEL <= 3
#define LOG_DEBUG(...) DBG_PRINTF(__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#if LOGLEVEL <= 4
#define LOG_INFO(...) DBG_PRINTF(__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#if LOGLEVEL <= 5
#define LOG_WARN(...) DBG_PRINTF(__VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#if LOGLEVEL <= 6
#define LOG_ERROR(...) DBG_PRINTF(__VA_ARGS__)
#else
#define LOG_ERROR(...)
#endif


#define ABORT_ON_ERROR(cond, ...) { \
	if(cond) { \
		DBG_PRINTF("ABORT_ON_ERROR @ %s:%d\n", __FILE__, __LINE__); \
		DBG_PRINTF(__VA_ARGS__); \
		exit(-1); \
	}\
}

#define WARN_ON_ERROR(cond, ...) { \
	if(cond) { \
		DBG_PRINTF("WARN @ %s:%d\n", __FILE__, __LINE__); \
		DBG_PRINTF(__VA_ARGS__); \
	} \
}

#define WARNONCE_ON_ERROR(cond, ...) { \
	static int check = 0; \
	if(!check) { \
		check = 1; \
		WARN_ON_ERROR(__VA_ARGS__); \
	} \
}

#define PIN_NUM 54

#endif //PLATFORM_DEPEND_INCLUDES_H
