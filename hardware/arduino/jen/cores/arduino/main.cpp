/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <Arduino.h>
extern "C" void vUTIL_UartText(char *pcString);

//Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (*func)()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

int main(void)
{
	init();

	initVariant();

#if defined(USBCON)
	USBDevice.attach();
#endif
	
	vUTIL_UartText("--- enter setup ---\r\n");
	setup();
	vUTIL_UartText("--- exit  setup ---\r\n");
    
	for (;;) {
		vUTIL_UartText("--- enter loop ---\r\n");
		loop();
		vUTIL_UartText("--- exit  loop ---\r\n");
//		if (serialEventRun) serialEventRun();
	}
        
	return 0;
}

extern "C"  void AppColdStart(void)
{
	main();
}

extern "C" void AppWarmStart(void)
{
	AppColdStart();
}
