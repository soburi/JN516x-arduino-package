#include "Arduino.h"
#include "wiring_private.h"

#ifdef __cplusplus
extern "C" {
#endif


void _analogReferenceDefault( eAnalogReference ulMode ) { (void)ulMode; }
void _analogWriteDefault( uint32_t ulPin, uint32_t ulValue ) { (void)ulPin; (void)ulValue; }
uint32_t _analogReadDefault( uint32_t ulPin ) { (void)ulPin; return 0; }
void _analogReadResolutionDefault(int res) { (void)res; }
void _analogWriteResolutionDefault(int res) { (void)res; }
void _analogOutputInitDefault( void ) { }

void analogReference( eAnalogReference ulMode ) __attribute__((weak,alias("_analogReferenceDefault")));
void analogWrite( uint32_t ulPin, uint32_t ulValue ) __attribute__((weak,alias("_analogWriteDefault")));
uint32_t analogRead( uint32_t ulPin ) __attribute__((weak,alias("_analogReadDefault")));
void analogReadResolution(int res) __attribute__((weak,alias("_analogReadResolutionDefault")));
void analogWriteResolution(int res) __attribute__((weak,alias("_analogWriteResolutionDefault")));
void analogOutputInit( void ) __attribute__((weak,alias("_analogOutputInitDefault")));



#ifdef __cplusplus
}
#endif
