#include "RingBufferStream.hpp"

RingBufferStream::RingBufferStream()
{
	memset( (void *)_aucBuffer, 0, SERIAL_BUFFER_SIZE ) ;
	_iHead=0 ;
	_iTail=0 ;
	observer = NULL;
}

size_t RingBufferStream::write_( const uint8_t uc_data )
{
	int l = (_iHead + 1) % SERIAL_BUFFER_SIZE;
	if ( l == _iTail )
	{
		if(observer) observer->buffer_overflowed();
		_iTail = (_iTail + 1) % SERIAL_BUFFER_SIZE;
	}
	_aucBuffer[_iHead] = uc_data;
	_iHead = l ;
	return 1;
}

size_t RingBufferStream::write( const uint8_t uc_data )
{
	int ret = write_(uc_data);
	if(observer) observer->data_arrival(1);
	return ret;
}

size_t RingBufferStream::write(const char *buffer, size_t size)
{
	for(uint32_t i=0; i<size; i++)
	{
		write_(buffer[i]);
	}
	if(observer) observer->data_arrival(size);
	return size;
}

int RingBufferStream::available()
{
	return (uint32_t)(SERIAL_BUFFER_SIZE + _iHead - _iTail) % SERIAL_BUFFER_SIZE;
}

int RingBufferStream::availableForWrite(void)
{
	if (_iHead >= _iTail) return SERIAL_BUFFER_SIZE - 1 - _iHead + _iTail;
	return _iTail - _iHead - 1;
}

int RingBufferStream::read( void )
{
	if ( _iHead == _iTail )
		return -1;
	
	uint8_t uc = _aucBuffer[_iTail];
	_iTail = (unsigned int)(_iTail + 1) % SERIAL_BUFFER_SIZE;
	return uc;
}

int RingBufferStream::peek( void )
{
	if ( _iHead == _iTail )
		return -1;
	return _aucBuffer[_iTail];
}

void RingBufferStream::flush( void )
{
}

