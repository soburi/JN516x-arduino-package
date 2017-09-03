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

#include "pin_location.h"

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

	switch(1<<ulPin) {
#if (PIN_ADC_1 == USE_ADC_1) || !(PIN_BITMAP & ~USE_ADC_1)
		case USE_ADC_1: adc = E_AHI_ADC_SRC_ADC_1; break;
#endif
#if (PIN_ADC_2 == USE_ADC_2) || !(PIN_BITMAP & ~USE_ADC_2)
		case USE_ADC_2: adc = E_AHI_ADC_SRC_ADC_2; break;
#endif
#if (PIN_ADC_4 == USE_ADC_4) || !(PIN_BITMAP & ~USE_ADC_4)
		case USE_ADC_4:  adc = E_AHI_ADC_SRC_ADC_3; break;
#endif
#if (PIN_ADC_3 == USE_ADC_3) || !(PIN_BITMAP & ~USE_ADC_3)
		case USE_ADC_3:  adc = E_AHI_ADC_SRC_ADC_4; break;
#endif
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
	switch(1<<ulPin) {
#if 0 // TIMER_0, TIMER_1 used by contiki.
#if (PIN_TIMER_0 == USE_TIMER_0
		case USE_TIMER_0:    timer = E_AHI_TIMER_0; break;
#endif
#if (PIN_TIMER_0 == USE_TIMER_0_ALT
		case USE_TIMER_0_ALT:    timer = E_AHI_TIMER_0; break;
#endif
#if (PIN_TIMER_1 == USE_TIMER_1
		case USE_TIMER_1:    timer = E_AHI_TIMER_1; break;
#endif
#if (PIN_TIMER_1 == USE_TIMER_1_ALT
		case USE_TIMER_1_ALT:    timer = E_AHI_TIMER_1; break;
#endif
#endif
#if (PIN_TIMER_2 == USE_TIMER_2) || !(PIN_BITMAP & ~USE_TIMER_2)
		case USE_TIMER_2: timer = E_AHI_TIMER_2; break;
#endif
#if (PIN_TIMER_2 == USE_TIMER_2_ALT)
		case USE_TIMER_2_ALT: timer = E_AHI_TIMER_2; break;
#endif
#if (PIN_TIMER_2 == USE_TIMER_2_RELOC)
		case USE_TIMER_2_RELOC: timer = E_AHI_TIMER_2; break;
#endif
#if (PIN_TIMER_3 == USE_TIMER_3) || !(PIN_BITMAP & ~USE_TIMER_3)
		case USE_TIMER_3: timer = E_AHI_TIMER_3; break;
#endif
#if (PIN_TIMER_3 == USE_TIMER_3_ALT)
		case USE_TIMER_3_ALT: timer = E_AHI_TIMER_3; break;
#endif
#if (PIN_TIMER_3 == USE_TIMER_3_RELOC)
		case USE_TIMER_3_RELOC: timer = E_AHI_TIMER_3; break;
#endif
#if (PIN_TIMER_4 == USE_TIMER_4) || !(PIN_BITMAP & ~USE_TIMER_4)
		case USE_TIMER_4: timer = E_AHI_TIMER_4; break;
#endif
#if (PIN_TIMER_4 == USE_TIMER_4_ALT)
		case USE_TIMER_4_ALT: timer = E_AHI_TIMER_4; break;
#endif
		default: return;
	}
	vAHI_TimerEnable(timer, 0, false, false, true);
	vAHI_TimerClockSelect(timer, false, true);
	vAHI_TimerConfigureOutputs(timer, true, true);
	vAHI_TimerStartRepeat(timer, 25000.0 * (ulValue/255.0), 25000);
}

