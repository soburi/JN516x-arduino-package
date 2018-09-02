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

#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include "platform.h"
#include "PinControl.h"
#include "wiring_digital.h"
#include "wiring_analog.h"

#include <unistd.h>

extern "C" {

static PinControl* pins[PIN_NUM];

void pinMode( uint32_t ulPin, uint32_t ulMode )
{
	int ret = 0;
	PinControl* pc = NULL;
	if( PwmPinControl::pwm_id(ulPin) >= 0) {
		pc = new PwmPinControl(ulPin);
		ABORT_ON_ERROR(pc==NULL, "PwmPinControl(%u) failed\n", ulPin);
	}
	else {
		pc = new GpioPinControl(ulPin);
		ABORT_ON_ERROR(pc==NULL, "GpioPinControl(%u) failed\n", ulPin);
	}

	
	ret = pc->exportPin();
	ABORT_ON_ERROR(ret<0, "PinControl::exportPin() pin=%u return:%d\n", ulPin, ret);

	ret = pc->setMode(ulMode);
	ABORT_ON_ERROR(ret<0, "PinControl::setMode(%u) return:%d\n", ulPin, ret);
	
	pins[ulPin] = pc;
	return;
}


void digitalWrite( uint32_t ulPin, uint32_t ulVal )
{
	if( PwmPinControl::pwm_id(ulPin) >= 0) return;

	LOG_DEBUG("%s(%u, %u)\n", __PRETTY_FUNCTION__, ulPin, ulVal);
	ABORT_ON_ERROR(pins[ulPin] == NULL, "pins[%u] is NULL\n", ulPin);

	int ret = pins[ulPin]->setValue(ulVal);
	WARN_ON_ERROR(ret<0, "PinControl::setValue(%u) pin=%u return:%d\n", ulVal, ulPin, ret);
}

int digitalRead( uint32_t ulPin )
{
	if( PwmPinControl::pwm_id(ulPin) >= 0) return 0;

	LOG_DEBUG("%s(%u)\n", __PRETTY_FUNCTION__, ulPin);
	ABORT_ON_ERROR(pins[ulPin] == NULL, "pins[%u] is NULL\n", ulPin);

	int ret = pins[ulPin]->getValue();
	WARN_ON_ERROR(ret<0, "PinControl::getValue() pin=%u return:%d\n", ulPin, ret);
	return ret;
}

void analogWrite( uint32_t ulPin, uint32_t ulValue )
{
	if( PwmPinControl::pwm_id(ulPin) < 0) return;

	if( pins[ulPin] == NULL) {
		pinMode(ulPin, OUTPUT);
	}

	LOG_DEBUG("%s(%u, %u)\n", __PRETTY_FUNCTION__, ulPin, ulValue);
	ABORT_ON_ERROR(pins[ulPin] == NULL, "pins[%u] is NULL\n", ulPin);

	int ret = pins[ulPin]->setValue(ulValue);
	WARN_ON_ERROR(ret<0, "PinControl::setValue(%u) pin=%u return:%d\n", ulValue, ulPin, ret);
}

void analogReference( eAnalogReference /*ulMode*/ )
{
	// not support in linux platform
}

uint32_t analogRead( uint32_t /*ulPin*/ )
{
	// not support in linux platform
	return 0;
}

void attachInterrupt(uint32_t ulPin, voidFuncPtr callback, uint32_t ulMode)
{
	LOG_DEBUG("%s\n", __FUNCTION__);
	if( PwmPinControl::pwm_id(ulPin) >= 0) return;

	pins[ulPin]->startInterruptPolling(ulMode, callback);

}

void detachInterrupt(uint32_t ulPin)
{
	LOG_DEBUG("%s\n", __FUNCTION__);
	if( PwmPinControl::pwm_id(ulPin) >= 0) return;

	pins[ulPin]->stopInterruptPolling();
}

}
