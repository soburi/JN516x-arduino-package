#pragma once

struct SERCOM;

struct SERCOM {
	int     (*input)( uint8_t c);
	void    (*init)(void*, uint32_t, uint8_t, uint8_t, uint8_t, uint8_t);
	void    (*set_input)(void*, int (*input)(unsigned char));
	void    (*writeb)(void*, unsigned char);
	uint8_t (*busy)(void*);
	void    (*deinit)(void*);
	void*	portinfo;
	uint8_t received;
};

typedef struct SERCOM SERCOM;

typedef uint8_t SercomRXPad;
typedef uint8_t SercomUartTXPad;
typedef uint8_t SercomNumberStopBit;
typedef uint8_t SercomUartCharSize;
typedef uint8_t SercomParityMode;
