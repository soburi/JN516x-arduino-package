/*
  Copyright (c) 2016 TOKITA Hiroshi.  All right reserved.

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

#ifdef USE_WIRING_ANALOG_PSEUDO_IMPLEMENT

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

#endif /* USE_WIRING_ANALOG_PSEUDO_IMPLEMENT */

