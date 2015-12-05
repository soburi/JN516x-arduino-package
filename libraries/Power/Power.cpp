#include "Power.h"
#include <AppHardwareApi.h>
#include <Arduino.h>

extern uint32_t ticktimer_overflow_count;

void PowerManager::delay(uint32_t ms)
{
	uint32_t now = millis();
	uint32_t last = now + ms;

	vAHI_WakeTimerEnable(SYSTEM_WAKETIMER, true);
	while( (u8AHI_UartReadLineStatus(E_AHI_UART_0)
		&  (E_AHI_UART_LS_THRE | E_AHI_UART_LS_TEMT))
		!= (E_AHI_UART_LS_THRE | E_AHI_UART_LS_TEMT) )
	{
		Serial.flush();
	}

	now = millis();

	while(now < last)
	{
		uint64_t timeout = MSEC2WTCOUNT(last-now);
		vAHI_WakeTimerStartLarge(SYSTEM_WAKETIMER, timeout);
		vAHI_CpuDoze();

		now = millis();
	}

	vAHI_WakeTimerStop(SYSTEM_WAKETIMER);
}

void PowerManager::sleep(uint32_t sleepmode, uint32_t ms)
{
	uint32_t wake_time = 0;

	if(sleepmode == E_AHI_SLEEP_OSCON_RAMOFF || sleepmode == E_AHI_SLEEP_OSCON_RAMON)
	{
		uint32_t now = millis();
		DBG_PRINTF("now %lu\r\n", now);

		if((UINT32_MAX - now) > ms)
		{
			wake_time = (now + ms);
			if (wake_time & 0x1)
			{
				wake_time |= 0x2;
			}
			wake_time &= 0xFFFFFFFE;
		}
		else
		{
			wake_time = ms - (UINT32_MAX-now);
			wake_time |= 0x00000001; //carry flag
		}

		if(ms != 0)
		{
			DBG_PRINTF("vAHI_WakeTimerStartLarge %llu %u %u\r\n", MSEC2WTCOUNT(ms), ms, wakeTimerCalibrationValue() );
			vAHI_WakeTimerEnable(SYSTEM_WAKETIMER, true);
			vAHI_WakeTimerStartLarge(SYSTEM_WAKETIMER, MSEC2WTCOUNT(ms) );
		}
	}

	DBG_PRINTF("vAHI_WriteNVData %lu\r\n", wake_time);
	vAHI_WriteNVData(3, wake_time);
	
	vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_DISABLE);
	vAHI_UartDisable(E_AHI_UART_0);
	vAHI_UartDisable(E_AHI_UART_1);
	if ( !(sleepmode == E_AHI_SLEEP_OSCON_RAMOFF || sleepmode == E_AHI_SLEEP_OSCON_RAMON) )
	{
		vAHI_WakeTimerStop(E_AHI_WAKE_TIMER_0);
		vAHI_WakeTimerStop(E_AHI_WAKE_TIMER_1);
	}

	vAHI_WatchdogStop();

	vAHI_Sleep((teAHI_SleepMode)sleepmode);

	while(true) ;
}


PowerManager Power;

