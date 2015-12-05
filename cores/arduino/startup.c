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

#include <stdio.h>

extern int arduino_main();

int contiki_argc = 0;
char **contiki_argv;

int main(int argc, char **argv)
{
	/* crappy way of remembering and accessing argc/v */
	contiki_argc = argc;
	contiki_argv = argv;

	/* native under windows is hardcoded to use the first one or two args */
	/* for wpcap configuration so this needs to be "removed" from         */
	/* contiki_args (used by the native-border-router) */
#if defined(_WIN32_) || defined(__CYGWIN__)
	contiki_argc--;
	contiki_argv++;
#ifdef UIP_FALLBACK_INTERFACE
	contiki_argc--;
	contiki_argv++;
#endif
#endif
	return arduino_main();
}
