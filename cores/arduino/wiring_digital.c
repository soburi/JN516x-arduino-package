#include "Arduino.h"
#include "wiring_private.h"

#include <AppHardwareApi.h>

void pinMode( uint32_t ulPin, uint32_t ulMode )
{
	if (ulMode == INPUT) {
		vAHI_DioSetDirection((1UL<<ulPin), 0);
		vAHI_DioSetPullup(0, (1UL<<ulPin));
		DBG_PRINTF("input  p:");
		DBG_PRINTF("%d\r\n", ulPin);
		DBG_PRINTF(" m:");
		DBG_PRINTF("%d\r\n", ulMode);
		DBG_PRINTF("\r\n");
	} else if (ulMode == INPUT_PULLUP) {
		vAHI_DioSetDirection(0, (1UL<<ulPin));
		vAHI_DioSetPullup((1UL<<ulPin), 0 );
		DBG_PRINTF("pullup p:");
		DBG_PRINTF("%d\r\n", ulPin);
		DBG_PRINTF(" m:");
		DBG_PRINTF("%d\r\n", ulMode);
		DBG_PRINTF("\r\n");

	} else {
		vAHI_DioSetDirection(0, (1UL<<ulPin) );
		DBG_PRINTF("output p:");
		DBG_PRINTF("%d\r\n", ulPin);
		DBG_PRINTF(" m:");
		DBG_PRINTF("%d\r\n", ulMode);
		DBG_PRINTF("\r\n");
	}
}

void digitalWrite( uint32_t ulPin, uint32_t ulVal )
{
	if (ulVal == LOW) {
		DBG_PRINTF("LOW  p:");
		DBG_PRINTF("%d\r\n", ulPin);
		DBG_PRINTF(" v:");
		DBG_PRINTF("%d\r\n", ulVal);
		DBG_PRINTF("\r\n");
		vAHI_DioSetOutput(0, (1UL<<ulPin));
	} else {
		DBG_PRINTF("HIGH p:");
		DBG_PRINTF("%d\r\n", ulPin);
		DBG_PRINTF(" v:");
		DBG_PRINTF("%d\r\n", ulVal);
		DBG_PRINTF("\r\n");
		vAHI_DioSetOutput((1UL<<ulPin), 0);
	}
}

int digitalRead( uint32_t ulPin )
{
	if (u32AHI_DioReadInput() & (1<<ulPin)) return HIGH;
	return LOW;
}
