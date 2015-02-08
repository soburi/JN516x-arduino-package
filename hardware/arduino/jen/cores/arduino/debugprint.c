#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <AppHardwareApi.h>
#include <stdarg.h>

#include "Arduino.h"

void debugprint(char *format, ...)
{
	static char buffer[64];
	static char* buf = buffer;
	va_list arg;
	va_start(arg, format);
	vsnprintf(buf, 64, format, arg);
	va_end(arg);
	while (*buf)
	{
        	while ((u8AHI_UartReadLineStatus(E_AHI_UART_0) & E_AHI_UART_LS_THRE) == 0); 
		vAHI_UartWriteData(E_AHI_UART_0, *buf);
		if (*buf == '\n')
		{
			vAHI_UartWriteData(E_AHI_UART_0, '\r');
		} 
		buf++;
	}
}


