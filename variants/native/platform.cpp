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

#include <stdint.h>
#include "variant.h"
#include "PinControl.h"

extern "C" {

void initVariant(void)
{
	Console.begin(0);
	Stderr.begin(0);

	char pwmstr[16];
	sprintf(pwmstr, "%d,%d", PWM0, PWM1);
	PwmPinControl::set_pwm_pins(pwmstr);
}

}
