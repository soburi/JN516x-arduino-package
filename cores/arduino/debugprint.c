#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <AppHardwareApi.h>

#include "Arduino.h"

#include "itoa.h"

static char buf_[33];

void debug_str(const char* buf) {
	while (*buf)
	{
		while ((u8AHI_UartReadLineStatus(E_AHI_UART_0) & E_AHI_UART_LS_THRE) == 0);
		vAHI_UartWriteData(E_AHI_UART_0, *buf);
		if (*buf == 0) {
			return;
		}
		if (*buf == '\n')
		{
			vAHI_UartWriteData(E_AHI_UART_0, '\r');
		}
		buf++;
	}
}

void debug_long(long val, int radix) {
	ltoa(val, buf_, 10);
	debug_str(buf_);
}
