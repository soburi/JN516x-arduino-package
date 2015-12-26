#include "Arduino.h"

#include <dev/leds.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t gpioPin2LedBits(uint32_t ulPin) __attribute__((weak));
uint32_t gpioPin2LedBits(uint32_t ulPin)
{
	return ulPin;
}

void _pinModeDefault( uint32_t ulPin, uint32_t ulMode )
{
}


void _digitalWriteDefault( uint32_t ulPin, uint32_t ulVal )
{
	if(ulVal == LOW)
	{
		leds_off( gpioPin2LedBits(ulPin) );
	}
	else
	{
		leds_on( gpioPin2LedBits(ulPin) );
	}
}


int _digitalReadDefault( uint32_t ulPin )
{
	if( leds_get() & gpioPin2LedBits(ulPin) )
	{
		return HIGH;
	}
	
	return LOW;
}

void pinMode( uint32_t ulPin, uint32_t ulMode ) __attribute__((weak, alias("_pinModeDefault")));
void digitalWrite( uint32_t ulPin, uint32_t ulVal ) __attribute__((weak, alias("_digitalWriteDefault")));
int digitalRead( uint32_t ulPin ) __attribute__((weak, alias("_digitalReadDefault")));
#ifdef __cplusplus
}
#endif

