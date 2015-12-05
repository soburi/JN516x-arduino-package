#include <stdio.h>

extern int arduino_main();

int contiki_argc = 0;
char **contiki_argv;

int main(int argc, char **argv)
{
#if NETSTACK_CONF_WITH_IPV6
#if UIP_CONF_IPV6_RPL
	fprintf(stderr,CONTIKI_VERSION_STRING " started with IPV6, RPL\n");
#else
	fprintf(stderr,CONTIKI_VERSION_STRING " started with IPV6\n");
#endif
#else
	fprintf(stderr,CONTIKI_VERSION_STRING " started\n");
#endif

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
	arduino_main();
}
