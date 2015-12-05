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

#include <stdint.h>

#include "Arduino.h"

extern int main(void);

typedef void (*funcptr)(); 
extern funcptr ctors_start;

void AppColdStart(void)
{
	// call global initializers.
	unsigned long* ptr = (unsigned long*)(&ctors_start);

	// Terminate with .end_ctors section header.
	while(*ptr) {
		funcptr fp = (funcptr)(*ptr++);
		fp();
	}
	main();
}

void AppWarmStart(void)
{
	main();
}


