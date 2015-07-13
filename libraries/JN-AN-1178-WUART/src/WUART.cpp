#include <sys/types.h>
#include <jendefs.h>
#include <stdint.h>
#include <stddef.h>

#include "JN_AN_1178.h"

#include "WUART.hpp"
#include <Arduino.h>
#include <Stream.h>

#ifdef IMPL_WUART

#define WUART_DBG_PRINTF(...) DBG_PRINTF(__VA_ARGS__)
#define WUART_DBG_PRINTLN(...) { DBG_PRINTF(__VA_ARGS__); DBG_PRINTF("\r\n"); }
#define WUART_DBG_NL() DBG_PRINTF("\r\n")

//#define WUART_TRACE_FUNC

#ifdef WUART_TRACE_FUNC
#define WUART_DBG_FUNC_ENTER() WUART_DBG_PRINTLN(__PRETTY_FUNCTION__)
#define WUART_DBG_FUNC_PARAM(...) WUART_DBG_PRINTF(__VA_ARGS__)
#define WUART_DBG_FUNC_PARAMLN(...) WUART_DBG_PRINTLN(__VA_ARGS__)
#define WUART_DBG_FUNC_NL() WUART_DBG_NL()
#else
#define WUART_DBG_FUNC_ENTER()
#define WUART_DBG_FUNC_PARAM(...)
#define WUART_DBG_FUNC_PARAMLN(...)
#define WUART_DBG_FUNC_NL()
#endif

#define WUART_TRACE_STATE

#ifdef WUART_TRACE_STATE
#define WUART_DBG_STATE(...) WUART_DBG_PRINTLN(__VA_ARGS__)
#else
#define WUART_DBG_STATE(...)
#endif

#define PR_DEBUG 1

#if PR_DEBUG
static char* EVENT_NAME(const char* funcname)
{
	if( strcmp(funcname, "handleTxAtIdle") == 0)
	{
		return "TX";
	}
	else if( strcmp(funcname, "handleTxAtNotIdle") == 0)
	{
		return "Expire";
	}
	else if( strcmp(funcname, "handleRxEnqAtNotTx") == 0)
	{
		return "RX(ENQ)";
	}
	else if( strcmp(funcname, "handleRxAckAtEnq") == 0)
	{
		return "RX(ACK)";
	}
	else if( strcmp(funcname, "handleRxAckAtTx") == 0)
	{
		return "RX(ACK)";
	}
	else if( strcmp(funcname, "handleRxNackAtEnq") == 0)
	{
		return "RX(NACK)";
	}
	else if( strcmp(funcname, "handleRxNackAtTx") == 0)
	{
		return "RX(NACK)";
	}
	else if( strcmp(funcname, "handleRxStxAtRx") == 0)
	{
		return "RX(STX)";
	}
	else if( strcmp(funcname, "handleRxNop") == 0)
	{
		return "RX(NOP)";
	}
	return "???";
}

static char* STATE_NAME[] = { "IDLE", "RX", "ENQ", "TX", "WAIT" };

#define SET_STATE(newstate) \
{ \
	(void)STATE_NAME; (void)EVENT_NAME; \
	WUART_DBG_PRINTLN("-- [%s]: %s->%s", EVENT_NAME(__func__), STATE_NAME[state], STATE_NAME[newstate]); \
	setState(newstate); \
}

#else
#define SET_STATE(newstate) \
{ \
	setState(newstate); \
}
#endif

#define PACKET_RESET(pkt) { (pkt).len = 0; }


WUARTStream::WUARTStream()
{
	WUART_DBG_FUNC_ENTER();
	//init state handler.
	event[STATE_IDLE].Tx = &WUARTStream::handleTxAtIdle;
	event[STATE_RX].Tx  = &WUARTStream::handleTxAtNotIdle;
	event[STATE_ENQ].Tx = &WUARTStream::handleTxAtNotIdle;
	event[STATE_TX].Tx = &WUARTStream::handleTxAtNotIdle;
	event[STATE_WAIT].Tx = &WUARTStream::handleTxAtNotIdle;

	event[STATE_IDLE].RxEnq = &WUARTStream::handleRxEnqAtNotTx;
	event[STATE_RX].RxEnq = &WUARTStream::handleRxEnqAtNotTx;
	event[STATE_ENQ].RxEnq = &WUARTStream::handleRxEnqAtNotTx;
	event[STATE_TX].RxEnq = &WUARTStream::handleRxNop;
	event[STATE_WAIT].RxEnq = &WUARTStream::handleRxEnqAtNotTx;

	event[STATE_IDLE].RxAck = &WUARTStream::handleRxNop;
	event[STATE_RX].RxAck = &WUARTStream::handleRxNop;
	event[STATE_ENQ].RxAck = &WUARTStream::handleRxAckAtEnq;
	event[STATE_TX].RxAck = &WUARTStream::handleRxAckAtTx;
	event[STATE_WAIT].RxAck = &WUARTStream::handleRxNop;

	event[STATE_IDLE].RxNack = &WUARTStream::handleRxNop;
	event[STATE_RX].RxNack = &WUARTStream::handleRxNop;
	event[STATE_ENQ].RxNack = &WUARTStream::handleRxNackAtEnq;
	event[STATE_TX].RxNack = &WUARTStream::handleRxNackAtTx;
	event[STATE_WAIT].RxNack = &WUARTStream::handleRxNop;

	event[STATE_IDLE].RxStx = &WUARTStream::handleRxNop;
	event[STATE_RX].RxStx  = &WUARTStream::handleRxStxAtRx;
	event[STATE_ENQ].RxStx = &WUARTStream::handleRxNop;
	event[STATE_TX].RxStx = &WUARTStream::handleRxNop;
	event[STATE_WAIT].RxStx = &WUARTStream::handleRxNop;

	destaddr = 0xFFFF;
}

void WUARTStream::Init(Protocol_Type nodetype, uint16_t addr, JN_AN_1178_Protocol* parent)
{
	WUART_DBG_FUNC_ENTER();
	state       = STATE_IDLE;
	busycount      = 0;
	seqTx = 0;
	seqRx = 0;
	isCoordinator       = nodetype;
	yield       = ! isCoordinator;
	wait        = false;
	maxPacketSize = 64;

	destaddr = addr;

	PACKET_RESET(dataPacket);
	PACKET_RESET(ctrlPacket);

	mParent = parent;

#if PR_DEBUG
	WUART_DBG_PRINTF("%04x Start\r\n", 0);
#endif
}

void WUARTStream::Tx()
{
	WUART_DBG_FUNC_ENTER();

	if(destaddr == 0xFFFF) return;

	(this->*event[state].Tx)();
}

void WUARTStream::Rx(uint8_t* buf, size_t bufLen)
{
	WUART_DBG_FUNC_ENTER();

	if(destaddr == 0xFFFF) return;

#if PR_DEBUG
	WUART_DBG_PRINTLN("");
	WUART_DBG_PRINTF(">> %04x %02x", destaddr, buf[0]);
	switch(buf[0])
	{
		case CTRL_ENQ:
		case CTRL_STX:
			WUART_DBG_PRINTF(" %02x", buf[1]);
			break;
	}
	WUART_DBG_PRINTLN("");
#endif

	switch(buf[0])
	{
		case CTRL_ENQ:
		{
			bool yieldOther = (bool) buf[1];
			if (yieldOther == yield)
			{
				yield = ! isCoordinator;
			}
			(this->*event[state].RxEnq)(buf, bufLen);
		}
		break;
		case CTRL_ACK:
		{
			(this->*event[state].RxAck)(buf, bufLen);
		}
		break;
		case CTRL_NACK:
		{
			(this->*event[state].RxNack)(buf, bufLen);
		}
		break;
		case CTRL_STX:
		{
			(this->*event[state].RxStx)(buf, bufLen);
		}
		break;
		default:
		break;
	}
}

void WUARTStream::handleRxNop(uint8_t* buf, size_t bufLen)
{
	WUART_DBG_FUNC_ENTER();
	(void)buf; (void)bufLen;
}

void WUARTStream::handleTxAtIdle()
{
	WUART_DBG_FUNC_ENTER();

	buildDataPacket(&dataPacket);
	if (dataPacket.len > 0)
	{
		SET_STATE(STATE_ENQ);
		buildCtrlPacket(&ctrlPacket, CTRL_ENQ);
		sendPacket(&ctrlPacket);
	}
}

void WUARTStream::handleTxAtNotIdle()
{
	WUART_DBG_FUNC_ENTER();

	busycount++;
	if (busycount > 25)
	{
		SET_STATE(STATE_IDLE);
	}
}

void WUARTStream::handleRxEnqAtNotTx(uint8_t* buf, size_t bufLen)
{
	WUART_DBG_FUNC_ENTER();
	(void)buf; (void)bufLen;

	if ( !(state == STATE_ENQ && yield == false) )
	{
		SET_STATE(STATE_RX);
		buildCtrlPacket(&ctrlPacket, CTRL_ACK);
		sendPacket(&ctrlPacket);
	}
	else
	{
		wait = true;
	}
}

void WUARTStream::handleRxAckAtEnq(uint8_t* buf, size_t bufLen)
{
	WUART_DBG_FUNC_ENTER();
	(void)buf; (void)bufLen;

	SET_STATE(STATE_TX);
	sendPacket(&dataPacket);
}

void WUARTStream::handleRxAckAtTx(uint8_t* buf, size_t bufLen)
{
	WUART_DBG_FUNC_ENTER();
	(void)buf; (void)bufLen;

	if (wait)
	{
		SET_STATE(STATE_WAIT);
		wait = false;
	}
	else
	{
		SET_STATE(STATE_IDLE);
	}

	PACKET_RESET(dataPacket);
	yield = true;
}

void WUARTStream::handleRxNackAtEnq(uint8_t* buf, size_t bufLen)
{
	WUART_DBG_FUNC_ENTER();
	(void)buf; (void)bufLen;

	SET_STATE(STATE_WAIT);
}

void WUARTStream::handleRxNackAtTx(uint8_t* buf, size_t bufLen)
{
	WUART_DBG_FUNC_ENTER();
	(void)buf; (void)bufLen;

	SET_STATE(STATE_WAIT);
	yield = true;
}

void WUARTStream::handleRxStxAtRx(uint8_t* buf, size_t bufLen)
{
	WUART_DBG_FUNC_ENTER();

	SET_STATE(STATE_IDLE);

	uint8_t recvSeq = buf[1];
	if (isNewRxSequence(recvSeq, seqRx))
	{
		seqRx = recvSeq + 1;
		if ( (size_t)rxbuffer.availableForWrite() > bufLen )
		{
			for (uint32_t i = 2; i < bufLen; i++)
			{
				rxbuffer.write(buf[i]);
			}

			buildCtrlPacket(&ctrlPacket, CTRL_ACK);
			sendPacket(&ctrlPacket);
		}
		else
		{
			buildCtrlPacket(&ctrlPacket, CTRL_NACK);
			sendPacket(&ctrlPacket);
		}
	}
	else
	{
		buildCtrlPacket( &ctrlPacket,CTRL_ACK);
		sendPacket(&ctrlPacket);
	}

	yield = false;
}

void WUARTStream::setState(Protocol_State eNewState)
{
	WUART_DBG_FUNC_ENTER();
	state = eNewState;
	busycount = 0;
}

bool WUARTStream::isNewRxSequence(uint8_t recvSeq, uint8_t aSeqRx)
{
	WUART_DBG_FUNC_ENTER();
	uint8_t  newSeq = aSeqRx + 128;
	if (newSeq >= aSeqRx)
	{
		if (recvSeq >= aSeqRx && recvSeq <= newSeq)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (recvSeq >= aSeqRx && recvSeq <= newSeq)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}

void WUARTStream::buildDataPacket(struct Packet* packet)
{
	WUART_DBG_FUNC_ENTER();
	if (packet->len != 0) return;
	if (!txbuffer.available())  return;

	packet->buf[packet->len++] = (uint8_t) CTRL_STX;
	packet->buf[packet->len++] = seqTx++;

	while ( txbuffer.available() && (packet->len < maxPacketSize))
	{
		packet->buf[packet->len++] = txbuffer.read();
	}
}

void WUARTStream::buildCtrlPacket(struct Packet* packet, char ctrlCode)
{
	WUART_DBG_FUNC_ENTER();

	PACKET_RESET(*packet);
	packet->buf[packet->len++] = (uint8_t) ctrlCode;
	if (ctrlCode == CTRL_ENQ)
	{
		packet->buf[packet->len++] = (uint8_t) yield;
	}
}

void WUARTStream::sendPacket(struct Packet* packet)
{
	WUART_DBG_FUNC_ENTER();

#if PR_DEBUG
	WUART_DBG_PRINTLN("");
	WUART_DBG_PRINTF("<< %04x %02x", 0, packet->buf[0]);
	switch(packet->buf[0])
	{
		case CTRL_ENQ:
		{
			WUART_DBG_PRINTF("%02x", packet->buf[1]);
		}
		break;
		case CTRL_STX:
		{
			uint8 i;
			WUART_DBG_PRINTF("%02x", packet->buf[1]);
			WUART_DBG_PRINTF("\r\n");
			for (i=2; i<packet->len; i++)
				WUART_DBG_PRINTF("%c", packet->buf[i]);
		}
		break;
	}
	WUART_DBG_PRINTLN("");
#endif

	mParent->data(destinationAddress(), packet->buf, packet->len);
}

#endif //IMPL_WUART
