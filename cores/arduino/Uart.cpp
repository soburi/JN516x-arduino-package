#include "Uart.h"

#include "Arduino.h"
#include <AppHardwareApi.h>
//#define DEBUGPRINT_DISABLE(x)

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
	int c_baud;
	switch(dwBaudRate)
	{
	case 4800:   c_baud = UART_RATE_4800;   break;
	case 9600:   c_baud = UART_RATE_9600;   break;
	case 19200:  c_baud = UART_RATE_19200;  break;
	case 38400:  c_baud = UART_RATE_38400;  break;
	case 76800:  c_baud = UART_RATE_76800;  break;
	case 115200: c_baud = UART_RATE_115200; break;
	default:     c_baud = -1;                     break;
	}

	uint32_t wordlen;
	switch( (config>>6) & 0x3)
	{
	case 0x0: wordlen = E_AHI_UART_WORD_LEN_5;  break;
	case 0x1: wordlen = E_AHI_UART_WORD_LEN_6;  break;
	case 0x2: wordlen = E_AHI_UART_WORD_LEN_7;  break;
	case 0x3: wordlen = E_AHI_UART_WORD_LEN_8;  break;
	default:  wordlen = -1; break;
	}

	uint32_t stopbit =
		( ((config>>12) & 0x01) ? E_AHI_UART_1_STOP_BIT : E_AHI_UART_1_STOP_BIT);
	uint32_t parity_enable =
		( ((config>>11) & 0x01) ? E_AHI_UART_PARITY_DISABLE : E_AHI_UART_PARITY_ENABLE);
	uint32_t parity_type =
		( ((config>>9) & 0x01) ? E_AHI_UART_EVEN_PARITY: E_AHI_UART_ODD_PARITY);


	DBG_PRINTF("c_baud:");
	DBG_PRINTF("%d\r\n", c_baud);
	DBG_PRINTF("\r\n");
	DBG_PRINTF("wordlen:");
	DBG_PRINTF("%d\r\n", wordlen);
	DBG_PRINTF("\r\n");
	DBG_PRINTF("stopbit:");
	DBG_PRINTF("%d\r\n", stopbit);
	DBG_PRINTF("\r\n");
	DBG_PRINTF("parity_enable:");
	DBG_PRINTF("%d\r\n", parity_enable);
	DBG_PRINTF("\r\n");
	DBG_PRINTF("parity_type:");
	DBG_PRINTF("%d\r\n", parity_type);
	DBG_PRINTF("\r\n");

	//vAHI_UartSetControl(_dwId, parity_type, parity_enable, wordlen, stopbit, false);
	sercom->set_input(sercom->input);
	sercom->init(c_baud);
}

void Uart::end( void )
{
	flush();
	vAHI_UartDisable(sercom->port);
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
	while (sercom->active() ) ;
}

size_t Uart::write( const uint8_t uc_data )
{
	sercom->writeb(uc_data);
	return 1;
}

void Uart::IrqHandler()
{
	rxBuffer.store_char(sercom->received);
}

