#ifndef SPIDRIVER_H__
#define SPIDRIVER_H__

#define SPI_TXBUF spi_txbuf_
#define SPI_RXBUF spi_rxbuf_

#define SPI_WAITFOREOTx() \
	do {\
		spi_write(SPI_TXBUF);\
	} while(0)

#define SPI_WAITFOREORx(); \
	do {\
		SPI_RXBUF = spi_read();\
	} while(0)

#include <stdint.h>
#include <stdbool.h>
#include <dev/spi.h>

extern uint8_t spi_txbuf_;
extern uint8_t spi_rxbuf_;

extern void spi_deinit();

extern void spi_configure(uint8_t slave_num, bool lsbFirst,
		bool polarity, bool phase, uint8_t clockDivider);

extern void spi_slave_select(uint8_t slave_bits);

extern void spi_start(int slave);
extern void spi_stop();

extern void spi_write(uint8_t txbuf);
extern void spi_write16(uint16_t txbuf);
extern void spi_write32(uint32_t txbuf);

extern uint8_t spi_read();
extern uint16_t spi_read16();
extern uint32_t spi_read32();

#endif /* SPIDRIVER_H__ */

