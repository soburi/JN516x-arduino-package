#ifndef WiringPrivate_h
#define WiringPrivate_h

#include <stdio.h>
#include <stdarg.h>

#include "Arduino.h"

#include <AppHardwareApi.h>

#ifdef __cplusplus
extern "C"{
#endif

extern PR_HWINT_APPCALLBACK SysCtrl_DIO_interrupt_handler;

#ifdef __cplusplus
}
#endif

#endif
