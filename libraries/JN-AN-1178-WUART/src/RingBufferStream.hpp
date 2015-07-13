#include <Stream.h>
#include <RingBuffer.h>

#ifndef RINGBUFFERSTREAM_HPP_
#define RINGBUFFERSTREAM_HPP_

class InputStreamObserver {
public:
	virtual void data_arrival(int) = 0;
	virtual void buffer_overflowed() = 0;
};


class RingBufferStream : public Stream {
  volatile uint8_t _aucBuffer[SERIAL_BUFFER_SIZE] ;
  volatile int _iHead ;
  volatile int _iTail ;
  
public:
  static void v_nop_i(int) { }
  static void v_nop() { }

  
  RingBufferStream();
  size_t write( const uint8_t uc_data );
  size_t write(const char *buffer, size_t size);
  int available();
  int read( void );
  int peek( void );
  void flush( void );
  int availableForWrite(void);

  void setObserver(InputStreamObserver& observer_) { observer = &observer_; }

private:
  size_t write_( const uint8_t uc_data );

  InputStreamObserver* observer;
  
};

#endif //RINGBUFFERSTREAM_HPP_

