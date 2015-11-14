#ifndef _VARIANT_PLATFORM_H
#define _VARIANT_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#undef F_CPU
#include <contiki-project-conf.h>
#include <contiki.h>
#include <net/netstack.h>
#include <dev/watchdog.h>
#include <dev/uart0.h>
#include <dev/uart1.h>
#include <lib/random.h>

typedef struct {
	uint8_t (*active)(void);
	void (*set_input)(int (*input)(unsigned char));
	void (*writeb)(unsigned char);
	void (*init)(uint8_t);
	int (*input)(uint8_t c);
	uint8_t port;
	uint8_t received;
} SERCOM;

typedef uint8_t SercomRXPad;
typedef uint8_t SercomUartTXPad;
typedef uint8_t SercomNumberStopBit;
typedef uint8_t SercomUartCharSize;
typedef uint8_t SercomParityMode;

typedef uint8_t IRQn_Type;

typedef uint8_t Pio;

#define ARDUINO_LOOP loop_

#define PUBLIC_PROCESS_THREAD(name, ev, data) 				\
PT_THREAD(process_thread_##name(struct pt *process_pt,	\
				       process_event_t ev,	\
				       process_data_t data))

#define LOOP() PUBLIC_PROCESS_THREAD(loop_, _ev, _data)

#define ARDUINO_LOOP_BEGIN LOOP() \
{ \
	(void)_ev; (void)_data;\
	PROCESS_BEGIN();

#define ARDUINO_LOOP_END \
	PROCESS_END();\
}


#ifdef __cplusplus
}
#endif

#endif //PLATFORM_DEPEND_INCLUDES_H
