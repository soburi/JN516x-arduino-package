/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "platform_spi.h"

SPIClass::SPIClass(Spi *_spi, uint32_t _id, void(*_initCb)(void)) :
	spi(_spi), id(_id), initCb(_initCb), initialized(false)
{
	DBG_PRINTF("SPIClass::SPIClass\r\n");
	// Empty
	id = -1;

	bitOrder[0] = bitOrder[1] = bitOrder[2] = bitOrder[3] = MSBFIRST;
	    mode[0] =     mode[1] =     mode[2] =     mode[3] = SPI_MODE0;
	 divider[0] =  divider[1] =  divider[2] =  divider[3] = SPI_CLOCK_DIV4;
}

void SPIClass::begin() {
	begin(BOARD_SPI_DEFAULT_SS);
}

void SPIClass::begin(uint8_t _pin) {
	uint32_t ch = BOARD_PIN_TO_SPI_CHANNEL(_pin);
	DBG_PRINTF("SPIClass::begin %x\r\n", ch);
	if(!initialized) init();
}

void SPIClass::init() {
	DBG_PRINTF("SPIClass::init %x %x %x %x \r\n", LSBFIRST==bitOrder[0], !bool(0x2&mode[0]), bool(0x1&mode[0]), divider[0]);
	vAHI_SpiConfigure(3, LSBFIRST==bitOrder[0], !bool(0x2&mode[0]), bool(0x1&mode[0]), divider[0],
			     E_AHI_SPIM_INT_DISABLE, E_AHI_SPIM_AUTOSLAVE_DSABL);
	tSpiConfiguration conf;
	vAHI_SpiReadConfiguration(&conf);
	DBG_PRINTF("   Read = %x\r\n", conf);
	initialized = true;
}

#ifndef interruptsStatus
#define interruptsStatus() __interruptsStatus()
static inline unsigned char __interruptsStatus(void) __attribute__((always_inline, unused));
static inline unsigned char __interruptsStatus(void) {
	unsigned int primask, faultmask;
	asm volatile ("mrs %0, primask" : "=r" (primask));
	if (primask) return 0;
	asm volatile ("mrs %0, faultmask" : "=r" (faultmask));
	if (faultmask) return 0;
	return 1;
}
#endif

void SPIClass::usingInterrupt(uint8_t interruptNumber)
{
	//TODO
}

void SPIClass::beginTransaction(uint8_t pin, SPISettings settings)
{
	//TODO
}

void SPIClass::endTransaction(void)
{
	//TODO
}

void SPIClass::end(uint8_t _pin) {
	// read - disable restore
}

void SPIClass::end() {
	vAHI_SpiDisable();
}

void SPIClass::setBitOrder(uint8_t _pin, BitOrder _bitOrder) {
	uint32_t ch = BOARD_PIN_TO_SPI_CHANNEL(_pin);
	DBG_PRINTF("SPIClass::setBitOrder %x\r\n", ch);
	bitOrder[ch] = _bitOrder;
}

void SPIClass::setDataMode(uint8_t _pin, uint8_t _mode) {
	uint32_t ch = BOARD_PIN_TO_SPI_CHANNEL(_pin);
	DBG_PRINTF("SPIClass::setDataMode %x\r\n", ch);
	mode[ch] = _mode;
}

void SPIClass::setClockDivider(uint8_t _pin, uint8_t _divider) {
	uint32_t ch = BOARD_PIN_TO_SPI_CHANNEL(_pin);
	DBG_PRINTF("SPIClass::setClockDivider %x\r\n", ch);
	divider[ch] = _divider;
}

byte SPIClass::transfer(byte _pin, uint8_t _data, SPITransferMode _mode) {
	//TODO transfer mode 
	uint32_t ch = BOARD_PIN_TO_SPI_CHANNEL(_pin);
	DBG_PRINTF("SPIClass::transfer %x\r\n", ch);
	if(id != ch) {
		tSpiConfiguration conf;
		vAHI_SpiReadConfiguration(&conf);
		DBG_PRINTF("   Read = %x\r\n", conf);
		uint8_t nmode =  ((0x2&mode[ch]) ? 0x00 : 0x2) | (0x1&mode[ch]);

		/**
		 * This is just a hacking.
		 * TODO finding correct spec for tSpiConfiguration.
		 * 31 30 29 28 27 26 25 24 |23 22 21 20 19 18 17 16
		 * [???] [--- divider ---] |[?????????] [sel] ?? In
		 * 15 14 13 12 11 10  9  8 | 7  6  5  4  3  2  1  0
		 * [??????] Au Or [???????]|[????????????] Po Ph ??
		 */
		conf = (conf & 0xFFFFEFFF) | (bitOrder[ch] == LSBFIRST ? 0x1000 : 0x0);
		conf = (conf & 0xFFFFFFF9) | (nmode << 1);
		conf = (conf & 0x00FFFFFF) | (divider[ch] << 24);
		DBG_PRINTF("Restore = %x\r\n", conf);
		vAHI_SpiRestoreConfiguration(&conf);
		id = ch;
	}
	DBG_PRINTF("vAHI_SpiSelect %x\r\n", ch);
	vAHI_SpiSelect(ch > 3 ? 0 : 1<<ch);
	tSpiConfiguration conf;
	vAHI_SpiReadConfiguration(&conf);
	DBG_PRINTF("   Read = %x\r\n", conf);

	DBG_PRINTF("vAHI_SpiStartTransfer\r\n");
	vAHI_SpiStartTransfer(8, _data);

	DBG_PRINTF("vAHI_SpiWaitBusy\r\n");
	vAHI_SpiWaitBusy();

	DBG_PRINTF("u8AHI_SpiReadTransfer8\r\n");
	uint8_t ret = u8AHI_SpiReadTransfer8();
	DBG_PRINTF("ret = %x\r\n", ret);
	vAHI_SpiStop();
	return ret;
}

void SPIClass::transfer(byte _pin, void *_buf, size_t _count, SPITransferMode _mode) {
	//TODO
}

void SPIClass::attachInterrupt(void) {
	// Should be enableInterrupt()
}

void SPIClass::detachInterrupt(void) {
	// Should be disableInterrupt()
}

#if SPI_INTERFACES_COUNT > 0
static void SPI_0_Init(void) {
}

SPIClass SPI(SPI_INTERFACE, SPI_INTERFACE_ID, SPI_0_Init);
#endif

