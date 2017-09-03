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

#include "pin_location.h"
#include <AppHardwareApi.h>
#include <stdbool.h>

void set_pin_location() {

#if (PIN_UART_0 == USE_UART_0)
   vAHI_UartSetLocation(E_AHI_UART_0, false);
#  if (PIN_UART_0 == USE_UART_0_RTSCTS)
//#    warning UART_0 use DIO[12,13,14,15]
     vAHI_UartSetRTSCTS(E_AHI_UART_0, true);
#  else
//#    warning UART_0 use DIO[14,15]
#  endif
#elif(PIN_UART_0 == USE_UART_0_ALT)
   vAHI_UartSetLocation(E_AHI_UART_0, true);
#  if (PIN_UART_0 == USE_UART_0_RTSCTS_ALT)
//#    warning UART_0 use DIO[4,5,6,7]
     vAHI_UartSetRTSCTS(E_AHI_UART_0, true);
#  else
//#    warning UART_0 use DIO[6,7]
#  endif
#endif

#if (PIN_UART_1 == USE_UART_1)
//#warning UART_1 use DIO[9,11]
    vAHI_UartSetLocation(E_AHI_UART_1, false);
#elif(PIN_UART_1 == USE_UART_1_ALT)
//#warning UART_1 use DIO[14,15]
    vAHI_UartSetLocation(E_AHI_UART_1, true);
#endif

#if (PIN_TIMER_0 == USE_TIMER_0)
    vAHI_TimerSetLocation(E_AHI_TIMER_0, false, false);
#elif(PIN_TIMER_0 == USE_TIMER_0_ALT)
    vAHI_TimerSetLocation(E_AHI_TIMER_0, true, false);
#endif

#if (PIN_TIMER_1 == USE_TIMER_1)
//#warning TIMER_1 use DIO[11]
    vAHI_TimerSetLocation(E_AHI_TIMER_1, false, false);
#elif(PIN_TIMER_1 == USE_TIMER_1_ALT)
//#warning TIMER_1 use DIO[5]
    vAHI_TimerSetLocation(E_AHI_TIMER_1, true, false);
#endif

#if (PIN_TIMER_2 == USE_TIMER_2)
//#warning TIMER_2 use DIO[12]
    vAHI_TimerSetLocation(E_AHI_TIMER_2, false, false);
#elif(PIN_TIMER_2 == USE_TIMER_2_ALT)
//#warning TIMER_2 use DIO[6]
    vAHI_TimerSetLocation(E_AHI_TIMER_2, true, false);
#elif(PIN_TIMER_2 == USE_TIMER_2_RELOC)
//#warning TIMER_2 use DO[0]
    vAHI_TimerSetLocation(E_AHI_TIMER_2, false, true);
#endif

#if (PIN_TIMER_3 == USE_TIMER_3)
//#warning TIMER_3 use DIO[13]
    vAHI_TimerSetLocation(E_AHI_TIMER_3, false, false);
#elif(PIN_TIMER_3 == USE_TIMER_3_ALT)
//#warning TIMER_3 use DIO[7]
    vAHI_TimerSetLocation(E_AHI_TIMER_3, true, false);
#elif(PIN_TIMER_3 == USE_TIMER_3_RELOC)
//#warning TIMER_3 use DO[1]
    vAHI_TimerSetLocation(E_AHI_TIMER_3, false, true);
#endif


#if (PIN_TIMER_4 == USE_TIMER_4)
//#warning TIMER_4 use DIO[17]
    vAHI_TimerSetLocation(E_AHI_TIMER_4, false, false);
#elif(PIN_TIMER_4 == USE_TIMER_4_ALT)
//#warning TIMER_4 use DIO[8]
    vAHI_TimerSetLocation(E_AHI_TIMER_4, true, false);
#endif


#if (PIN_PC_0 == USE_PC_0)
//#warning PC_0 use DIO[5]
    vAHI_PulseCounterSetLocation(E_AHI_PC_0, false);
#elif (PIN_PC_0 == USE_PC_0_ALT)
//#warning PC_0 use DIO[4]
    vAHI_PulseCounterSetLocation(E_AHI_PC_0, true);
#endif

#if (PIN_PC_1 == USE_PC_1)
//#warning PC_1 use DIO[8]
    vAHI_PulseCounterSetLocation(E_AHI_PC_1, false);
#elif (PIN_PC_1 == USE_PC_1_ALT)
//#warning PC_1 use DIO[1]
    vAHI_PulseCounterSetLocation(E_AHI_PC_1, true);
#endif

#if (PIN_SI == USE_SI)
//#warning SI use DIO[14,15]
    vAHI_SiSetLocation(false);
#elif (PIN_SI == USE_SI_ALT)
//#warning SI use DIO[16,17]
    vAHI_SiSetLocation(true);
#endif

#if   (PIN_SPIM == USE_SPIM)
#  if   (PIN_SPIM == USE_SPIM_SEL_1)
#    if   (PIN_SPIM == USE_SPIM_SEL_1_SEL_2)
//#      warning SPIM use DIO[14,15,18,19], D0[0,1]
       vAHI_SpiSelSetLocation(E_AHI_SPISEL_2, false);
#    elif (PIN_SPIM == USE_SPIM_SEL_1_SEL_2_ALT)
//#      warning SPIM use DIO[1,15,18,19], D0[0,1]
       vAHI_SpiSelSetLocation(E_AHI_SPISEL_2, true);
#    else
//#      warning SPIM use DIO[14,18,19], D0[0,1]
       vAHI_SpiSelSetLocation(E_AHI_SPISEL_1, false);
#    endif //SEL_2
#  elif   (PIN_SPIM == USE_SPIM_SEL_1_ALT)
#    if   (PIN_SPIM == USE_SPIM_SEL_1_ALT_SEL_2)
//#      warning SPIM use DIO[0,15,18,19], D0[0,1]
       vAHI_SpiSelSetLocation(E_AHI_SPISEL_2, false);
#    elif (PIN_SPIM == USE_SPIM_SEL_1_ALT_SEL_2_ALT)
//#      warning SPIM use DIO[0,1,18,19], D0[0,1]
       vAHI_SpiSelSetLocation(E_AHI_SPISEL_2, true);
#    else
//#      warning SPIM use DIO[0,18,19], D0[0,1]
       vAHI_SpiSelSetLocation(E_AHI_SPISEL_1, true);
#    endif //SEL_2
#  else
//#    warning SPIM use DIO[18,19], D0[0,1]
#  endif//SEL_1
#endif

}


#if (PIN_JTAG + PIN_UART_0 + PIN_UART_1 + PIN_TIMER_0 + PIN_TIMER_1 + PIN_TIMER_2 + \
     PIN_TIMER_3 + PIN_TIMER_4 + PIN_PC_0 + PIN_PC_1 + PIN_SI + PIN_SPIM + \
     PIN_SPIS + PIN_ADC_1 + PIN_ADC_2 + PIN_ADC_3 + PIN_ADC_4 + PIN_COMP + \
     PIN_VREF + PIN_ANT_DIV + PIN_HIGHPOWER_MOD + \
     PIN_DIO_0 + PIN_DIO_1 + PIN_DIO_2 + PIN_DIO_3 + PIN_DIO_4 + PIN_DIO_5 + \
     PIN_DIO_6 + PIN_DIO_7 + PIN_DIO_8 + PIN_DIO_9 + PIN_DIO_10 + PIN_DIO_11 + \
     PIN_DIO_12 + PIN_DIO_13 + PIN_DIO_14 + PIN_DIO_15 + PIN_DIO_16 + PIN_DIO_17 + \
     PIN_DIO_18 + PIN_DIO_19 + PIN_DIO_20) != \
    (PIN_JTAG ^ PIN_UART_0 ^ PIN_UART_1 ^ PIN_TIMER_0 ^ PIN_TIMER_1 ^ PIN_TIMER_2 ^ \
     PIN_TIMER_3 ^ PIN_TIMER_4 ^ PIN_PC_0 ^ PIN_PC_1 ^ PIN_SI ^ PIN_SPIM ^ \
     PIN_SPIS ^ PIN_ADC_1 ^ PIN_ADC_2 ^ PIN_ADC_3 ^ PIN_ADC_4 ^ PIN_COMP ^ \
     PIN_VREF ^ PIN_ANT_DIV ^ PIN_HIGHPOWER_MOD ^ \
     PIN_DIO_0 ^ PIN_DIO_1 ^ PIN_DIO_2 ^ PIN_DIO_3 ^ PIN_DIO_4 ^ PIN_DIO_5 ^ \
     PIN_DIO_6 ^ PIN_DIO_7 ^ PIN_DIO_8 ^ PIN_DIO_9 ^ PIN_DIO_10 ^ PIN_DIO_11 ^ \
     PIN_DIO_12 ^ PIN_DIO_13 ^ PIN_DIO_14 ^ PIN_DIO_15 ^ PIN_DIO_16 ^ PIN_DIO_17 ^ \
     PIN_DIO_18 ^ PIN_DIO_19 ^ PIN_DIO_20)
#warning Pin settings conflict.
#endif

