#include "Uart.h"

#include "Arduino.h"

Uart::Uart(SERCOM *_s, uint8_t /*_pinRX*/, uint8_t /*_pinTX*/, SercomRXPad /*_padRX*/, SercomUartTXPad /*_padTX*/)
{
	sercom=_s;
}

void Uart::begin(unsigned long dwBaudRate)
{
	begin(dwBaudRate, (uint8_t)SERIAL_8N1);
}

void Uart::begin(unsigned long dwBaudRate, uint8_t config)
{
	uint8_t parity  =  config && HARDSER_PARITY_MASK;
	uint8_t stopbit = (config && HARDSER_STOP_BIT_MASK) >> 8;
	uint8_t wordlen = (config && HARDSER_DATA_MASK)     >> 16;
	uint8_t flowctrl = 0;
	
	sercom->set_input(sercom->portinfo, sercom->input);
	sercom->init(sercom->portinfo, dwBaudRate, parity, stopbit, wordlen, flowctrl);
}

void Uart::end( void )
{
	flush();
	sercom->deinit(sercom->portinfo);
}

int Uart::available( void )
{
	return (uint32_t)(SERIAL_BUFFER_SIZE + rxBuffer._iHead - rxBuffer._iTail) % SERIAL_BUFFER_SIZE;
}

int Uart::peek( void )
{
	return rxBuffer.peek();
}

int Uart::read( void )
{
	return rxBuffer.read_char();
}

void Uart::flush( void )
{
	while (sercom->busy(sercom->portinfo) ) ;
}

size_t Uart::write( const uint8_t uc_data )
{
	sercom->writeb(sercom->portinfo, uc_data);
	return 1;
}

void Uart::IrqHandler()
{
	rxBuffer.store_char(sercom->received);
}

