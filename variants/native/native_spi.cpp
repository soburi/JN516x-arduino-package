/*
  Copyright (c) 2016 Tokita, Hiroshi  All right reserved.

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

#include <cerrno>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "native_spi.h"

const SPISettings DEFAULT_SPI_SETTINGS = SPISettings();
const static uint8_t spiBPW = 8;
const static uint16_t spiDelay = 0;
const static uint8_t spiCSChange = 1;

SPIClass::SPIClass(const SPIDevFile* devfile)
	: spi()
	, bitOrder()
	, clock()
	, mode()
	, fd()
	, spiDev(devfile)
	, initialized()
{
}

void SPIClass::begin()
{
	LOG_DEBUG("SPIClass::begin\n");
	int ret = open(spiDev[0].path, O_RDWR);
	ABORT_ON_ERROR(ret<0, "open(%s) failure:%d %s\n", spiDev[0].path, errno, strerror(errno));

	this->fd = ret;
	init();
	config(DEFAULT_SPI_SETTINGS);
}

void SPIClass::init()
{
	int ret = 0;

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW);
	ABORT_ON_ERROR(ret<0, "SPI BPW Change failure:%d %s\n", errno, strerror(errno)) ;

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &clock);
	ABORT_ON_ERROR(ret<0, "SPI Speed Change failure:%d %s\n", errno, strerror(errno)) ;
}

void SPIClass::config(SPISettings settings)
{
	int ret = 0;
	LOG_DEBUG("SPIClass::begin %d %d\n", settings.clock, settings.mode);
	this->clock = settings.clock;
	this->mode  = settings.mode;

	uint8_t lsbfirst = (settings.order == MSBFIRST ? 0 : SPI_LSB_FIRST);

	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	ABORT_ON_ERROR(ret<0, "ioctl(SPI_IOC_WR_MODE) failure:%d %s\n",errno, strerror(errno) );
	ret = ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsbfirst);
	WARN_ON_ERROR(ret<0, "ioctl(SPI_IOC_WR_LSB_FIRST) failure:%d %s\n", errno, strerror(errno));
}

void SPIClass::end()
{
	close(fd);
}

void SPIClass::usingInterrupt(int /*interruptNumber*/) { }

void SPIClass::beginTransaction(SPISettings settings)
{
	LOG_DEBUG("SPIClass::beginTransaction %d %d\n", settings.clock, settings.mode);
	config(settings);
}

void SPIClass::endTransaction(void)
{
}

void SPIClass::setBitOrder(BitOrder order)
{
	uint8_t lsbfirst = (order == MSBFIRST ? 0 : SPI_LSB_FIRST);
	int ret = ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsbfirst);
	WARN_ON_ERROR(ret<0, "ioctl(SPI_IOC_WR_LSB_FIRST) failure:%d %s\n", errno, strerror(errno));
}

void SPIClass::setDataMode(uint8_t mode)
{
	this->mode = mode;
	int ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	WARN_ON_ERROR(ret<0, "ioctl(SPI_IOC_WR_MODE) failure:%d %s\n", errno, strerror(errno));
}

void SPIClass::setClockDivider(uint8_t div)
{
	this->clock = 16000000 / div;
}

byte SPIClass::transfer(uint8_t data)
{
	LOG_DEBUG("SPIClass::transfer %x\n", data);
	transfer(&data, 1);
	return data;
}

uint16_t SPIClass::transfer16(uint16_t data)
{
	LOG_DEBUG("SPIClass::transfer16 %x\n", data);
	transfer(&data, 2);
	return data;
}

void SPIClass::transfer(void *buf, size_t count)
{
	struct spi_ioc_transfer spi;

	spi.tx_buf        = (unsigned long)buf;
	spi.rx_buf        = (unsigned long)buf;
	spi.len           = count;
	spi.delay_usecs   = spiDelay;
	spi.speed_hz      = this->clock;
	spi.bits_per_word = spiBPW;
	spi.cs_change     = spiCSChange;

	int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &spi) ;
	WARN_ON_ERROR(ret<0, "ioctl(SPI_IOC_MESSAGE) failure:%d %s\n", errno, strerror(errno));
}

void SPIClass::attachInterrupt() { }

void SPIClass::detachInterrupt() { }

static const SPIDevFile devfile[] = { SPIDevFile("/dev/spidev0.0", 1), SPIDevFile("/dev/spidev0.1", 2) };

SPIClass SPI(devfile);
