#include "spi-driver.h"
#include <AppHardwareApi.h>
#include "platform.h"

extern uint8_t spi_txbuf_;
extern uint8_t spi_rxbuf_;

static size_t last_tx_bitlen;

void spi_init()
{
	spi_configure(0, 0, 0, 0, 8);
	spi_slave_select(0);
}

void spi_deinit()
{
	vAHI_SpiDisable();
}

void spi_configure(uint8_t slave_num, bool lsb_first,
		bool polarity, bool phase, uint8_t divider)
{
	DBG_PRINTF("vAHI_SpiConfigure(%d, %d %d, %d, %d, INT_DISABLE, AUTOSLAVE_DSABL)\n",
			slave_num, lsb_first, polarity, phase, divider,
			     E_AHI_SPIM_INT_DISABLE, E_AHI_SPIM_AUTOSLAVE_DSABL);
	vAHI_SpiConfigure(slave_num, lsb_first, polarity, phase, divider,
			     E_AHI_SPIM_INT_DISABLE, E_AHI_SPIM_AUTOSLAVE_DSABL);
}

void spi_slave_select(uint8_t slave_bits)
{
	vAHI_SpiSelect(slave_bits);
}

void spi_start(int slave)
{
	vAHI_SpiSelect(1<<slave);
}

void spi_stop()
{
	spi_slave_select(0);
}

static void spi_write_bits(uint32_t txbuf, size_t bitlen)
{
	if(bitlen <= 0)
		bitlen = 1;
	if(bitlen > 32)
		bitlen = 32;
	DBG_PRINTF("vAHI_SpiStartTransfer(%d, %08x)\n", bitlen-1, txbuf);
	vAHI_SpiStartTransfer(bitlen-1, txbuf);
	vAHI_SpiWaitBusy();
	last_tx_bitlen = bitlen;
}

void spi_write(uint8_t txbuf)
{
	spi_write_bits(txbuf, 8);
}

void spi_write16(uint16_t txbuf)
{
	spi_write_bits(txbuf, 16);
}

void spi_write32(uint32_t txbuf)
{
	spi_write_bits(txbuf, 32);
}
#if 0
static uint32_t spi_read_bits(size_t bitlen)
{
	size_t i = 0;
	uint32_t mask = 0;
	if(bitlen <= 0)
		bitlen = 1;
	if(bitlen > 32)
		bitlen = 32;
	for(i=0; i<bitlen; i++) {
		mask |= (1<<i);
	}
	uint32_t ret = u32AHI_SpiReadTransfer32() & mask;
	return ret;
}
#endif

uint8_t spi_read()
{
	uint32_t ret = u32AHI_SpiReadTransfer32() & 0xFF;
	DBG_PRINTF("spi_read() %x\n", ret);
	return ret & 0xFF;
}

uint16_t spi_read16()
{
	uint32_t ret = u32AHI_SpiReadTransfer32() & 0xFF;
	DBG_PRINTF("spi_read16() %x\n", ret);
	return ret & 0xFFFF;
}

uint32_t spi_read32()
{
	uint32_t ret = u32AHI_SpiReadTransfer32() & 0xFF;
	DBG_PRINTF("spi_read32() %x\n", ret);
	return ret;
}
