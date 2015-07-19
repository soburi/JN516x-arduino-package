#include <AppHardwareApi.h>
#include <Arduino.h>

static const uint32_t SLEEP			= E_AHI_SLEEP_OSCON_RAMON;
static const uint32_t SLEEP_RAM_OFF		= E_AHI_SLEEP_OSCON_RAMOFF;
static const uint32_t SLEEP_OSCILLATOR_OFF	= E_AHI_SLEEP_OSCOFF_RAMON;
static const uint32_t SLEEP_RAM_OSCILLATOR_OFF	= E_AHI_SLEEP_OSCOFF_RAMOFF;
static const uint32_t SLEEP_DEEP		= E_AHI_SLEEP_DEEP;

class PowerManager
{
public:

	void delay(uint32_t ms);

	void sleep(uint32_t sleepmode, uint32_t ms=0);
	bool isWarmBoot() { return warmBoot(); }

protected:
	uint32_t status() { u16AHI_PowerStatus(); }
};

extern PowerManager Power;

