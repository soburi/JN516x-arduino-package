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

#include "Arduino.h"
#include "wiring_private.h"

#include <stdbool.h>

#include <AppHardwareApi.h>
void analogReference( eAnalogReference ulMode )
{
	(void)ulMode;
	vAHI_ApConfigure(E_AHI_AP_REGULATOR_ENABLE, E_AHI_AP_INT_DISABLE, 
	                 E_AHI_AP_SAMPLE_2, E_AHI_AP_CLOCKDIV_500KHZ,
			 E_AHI_AP_INTREF);
			 //ulMode == EXTERNAL ? E_AHI_AP_INTREF : E_AHI_AP_EXTREF);
	while( !bAHI_APRegulatorEnabled() ) {}
}

uint32_t analogRead( uint32_t ulPin )
{
	DBG_PRINTF("analogRread:\r\n");

	uint32_t adc = 0;
	switch(ulPin) {
		case 0:  adc = E_AHI_ADC_SRC_ADC_3; break;
		case 1:  adc = E_AHI_ADC_SRC_ADC_4; break;
		case 0xA1: adc = E_AHI_ADC_SRC_ADC_1; break;
		case 0xA2: adc = E_AHI_ADC_SRC_ADC_2; break;
		default: return -1;
	}

	vAHI_AdcEnable(E_AHI_ADC_SINGLE_SHOT, E_AHI_AP_INPUT_RANGE_2, adc);
	vAHI_AdcStartSample();
	while(!bAHI_AdcPoll());

	uint16_t read;
	read = u16AHI_AdcRead();

	DBG_PRINTF("read:");
	DBG_PRINTF("%d\r\n", read);
	DBG_PRINTF("\r\n");
	return read;
}

void analogWrite( uint32_t ulPin, uint32_t ulValue )
{
	DBG_PRINTF("analogWrite ");
	DBG_PRINTF("%d\r\n", ulPin);
	DBG_PRINTF(" ");
	DBG_PRINTF("%d\r\n", ulValue);
	DBG_PRINTF("\r\n");

	uint32_t timer = 0;
	switch(ulPin) {
		case 5:    timer = E_AHI_TIMER_1; break;
		case 8:    timer = E_AHI_TIMER_4; break;
		case 0xD0: timer = E_AHI_TIMER_2; break;
		case 0xD1: timer = E_AHI_TIMER_3; break;
		default: return;
	}
	vAHI_TimerEnable(timer, 0, false, false, true);
	vAHI_TimerClockSelect(timer, false, true);
	vAHI_TimerConfigureOutputs(timer, true, true);
	vAHI_TimerStartRepeat(timer, 25000.0 * (ulValue/255.0), 25000);
}

