#ifndef  WUART_H_
#define  WUART_H_

#include <stdint.h>
#include <stddef.h>
#include <Stream.h>
#include "RingBufferStream.hpp"

#ifndef MAX_DATA_PER_FRAME
#define MAX_DATA_PER_FRAME 64
#endif

class JN_AN_1178_Protocol;

class WUARTStream : public Stream
{
friend class JN_AN_1178_Protocol;
	/** Data packet structure. */
	struct Packet
	{
		uint8_t buf[MAX_DATA_PER_FRAME];
		size_t len;
	};


	enum Protocol_State {
		STATE_IDLE=0,
		STATE_RX,
		STATE_ENQ,
		STATE_TX,
		STATE_WAIT,
		NUM_STATE
	};

	enum Protocol_Ctrl {
		CTRL_STX = 0x02,
		CTRL_ENQ = 0x05,
		CTRL_ACK = 0x06,
		CTRL_NACK = 0x15
	};

	enum Protocol_Type {
		TYPE_ENDD = 0,
		TYPE_COORDINATOR
	};

	typedef void (WUARTStream::*fpEventTx)();
	typedef void (WUARTStream::*fpEventRx)(uint8_t*, size_t);

	Protocol_State state;
	uint8_t busycount;			/**< State timer */
	uint8_t seqRx;
	uint8_t seqTx;
	Protocol_Type isCoordinator;
	bool  yield;
	bool  wait;
	struct Packet dataPacket;
	struct Packet ctrlPacket;

	struct {
		WUARTStream::fpEventTx Tx;
		WUARTStream::fpEventRx RxEnq;
		fpEventRx RxAck;
		fpEventRx RxNack;
		fpEventRx RxStx;
	} event[NUM_STATE];

	size_t maxPacketSize;

	JN_AN_1178_Protocol* mParent;
	RingBufferStream rxbuffer;
	RingBufferStream txbuffer;

	uint16_t destaddr;

public:
	inline size_t write( const uint8_t uc_data ) { return txbuffer.write(uc_data); }
	inline size_t write(const char *buffer, size_t size) { return txbuffer.write(buffer, size); }
	inline int available() { return rxbuffer.available(); }
	inline int read( void ) { return rxbuffer.read(); }
	inline int peek( void ) { return rxbuffer.peek(); }
	inline void flush( void ) { txbuffer.flush(); }

	inline uint16_t destinationAddress() const { return destaddr; }

private:
	WUARTStream();
	void Init(Protocol_Type, uint16_t, JN_AN_1178_Protocol*);
	void Rx(uint8_t*, size_t);
	void Tx();

private:
	void setState(Protocol_State eNewState);
	void buildDataPacket(struct Packet*);
	void buildCtrlPacket(struct Packet*, char);
	void sendPacket(struct Packet* packet);
	static bool isNewRxSequence(uint8_t, uint8_t);

	void handleTxAtIdle();
	void handleTxAtNotIdle();
	void handleRxEnqAtNotTx(uint8_t*, size_t);
	void handleRxAckAtEnq(uint8_t*, size_t);
	void handleRxAckAtTx(uint8_t*, size_t);
	void handleRxNackAtEnq(uint8_t*, size_t);
	void handleRxNackAtTx(uint8_t*, size_t);
	void handleRxNop(uint8_t*, size_t);
	void handleRxStxAtRx(uint8_t*, size_t);
};

#endif
