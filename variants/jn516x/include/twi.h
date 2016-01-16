#ifndef TWI_H
#define TWI_H

#include <AppHardwareApi.h>

#define WIRE_INTERFACES_COUNT 1
#define SI_CLOCK2PRESCALER(clk) ( (16L/((clk)/1000000L)/5L)-1 )


class Twi {
};

#endif
