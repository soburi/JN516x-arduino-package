#include "Arduino.h"

#include "wiring_private.h"

#define TICK_TIMER_MAX (0x0fffffff)

/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */

extern uint32_t heap_location;

void init( void )
{
	uint32_t ticktimer_start = 0;
	uint64_t wt_read = 0ull;
	uint64_t wake_time = 0ull;

	if( !warmBoot() && waked() )
	{
		DBG_PRINTF("WARMBOOT %x %x\r\n", u16AHI_PowerStatus(), u32AHI_ReadNVData(3) );
		wake_time = u32AHI_ReadNVData(3);
		if (u8AHI_WakeTimerFiredStatus() & SYSTEM_WAKETIMER_MASK)
		{
			DBG_PRINTF("Fired %x\r\n", u16AHI_PowerStatus());
			wt_read = 0ull;
		}
		else if (u8AHI_WakeTimerStatus() & SYSTEM_WAKETIMER_MASK)
		{
			DBG_PRINTF("Running %x\r\n", u16AHI_PowerStatus());
			wt_read = u64AHI_WakeTimerReadLarge(SYSTEM_WAKETIMER);
		}

		goto init_systimer;
	}

	u32AHI_Init();

	//SysCtrl
	vAHI_SysCtrlRegisterCallback(sysctrl_callback);

	//UART
#if !defined(DBG_ENABLE) || ( defined(DBG_UART) && (DBG_UART != DBG_E_UART_0) )
	vAHI_UartDisable(E_AHI_UART_0);
#endif
#if !defined(DBG_ENABLE) || ( DBG_UART != DBG_E_UART_1 )
	vAHI_UartDisable(E_AHI_UART_1);
#endif

	//PWM Timer
	vAHI_TimerSetLocation(E_AHI_TIMER_1, true, true);
	vAHI_TimerSetLocation(E_AHI_TIMER_2, true, true);
	vAHI_TimerSetLocation(E_AHI_TIMER_3, true, true);
	vAHI_TimerSetLocation(E_AHI_TIMER_4, true, true);
	vAHI_TimerFineGrainDIOControl(0x87);

	//ADC
	vAHI_ApConfigure(E_AHI_AP_REGULATOR_ENABLE, E_AHI_AP_INT_DISABLE,
	                 E_AHI_AP_SAMPLE_2, E_AHI_AP_CLOCKDIV_500KHZ, E_AHI_AP_INTREF);
	while( !bAHI_APRegulatorEnabled() ) {}

init_systimer:
	//WakeTimer
	vAHI_WakeTimerStop(E_AHI_WAKE_TIMER_0);
	vAHI_WakeTimerStop(E_AHI_WAKE_TIMER_1);
	(void)u8AHI_WakeTimerFiredStatus();
	setWakeTimerCalibrationValue( u32AHI_WakeTimerCalibrate() );

	//calc timer continued
	if(wake_time & 0x1)
	{
		wake_time |= (0x1ull<<32);
	}
	wake_time -= WTCOUNT2MSEC(wt_read);
	wake_time *= clockCyclesPerMicrosecond();
	ticktimer_overflow_count = (wake_time * 1000) / 0x0FFFFFFF;
	ticktimer_start = (wake_time * 1000) % 0x0FFFFFFF;

	DBG_PRINTF("ticktimer_set %d %d\r\n", ticktimer_overflow_count, ticktimer_start);

	//Tick Timer
	vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_DISABLE);
	vAHI_TickTimerWrite(ticktimer_start);
	vAHI_TickTimerInterval(TICK_TIMER_MAX);
	vAHI_TickTimerIntEnable(true);
	vAHI_TickTimerRegisterCallback(ticktimer_callback);
	vAHI_TickTimerConfigure(E_AHI_TICK_TIMER_RESTART);
}


