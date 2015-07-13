/*
 * JN_AN_1178.h
 *
 *  Created on: 2015/03/27
 *      Author: crs
 */

#ifndef JN_AN_1178_H_
#define JN_AN_1178_H_

#include <Stream.h>
#include "IEEE802154.hpp"
#include "WUART.hpp"

#define IMPL_WUART

#ifndef MAX_WUART
#define MAX_WUART 5
#endif

class JN_AN_1178_Protocol;
class WUARTStream;

extern JN_AN_1178_Protocol JN_AN_1178;

class JN_AN_1178_Protocol : public IEEE802154 {
	class WUART_Accessor {
	friend class JN_AN_1178_Protocol;
		JN_AN_1178_Protocol& m_Protocol;
		WUART_Accessor(JN_AN_1178_Protocol& protocol);
	public:
		WUARTStream& operator()(int) const;
		WUARTStream& operator[](int) const;
	};


friend class JN_AN_1178_Protocol::WUART_Accessor;
friend class WUARTStream;

public:
	const WUART_Accessor WUART;

private:
	enum {
		STATE_INIT,
		STATE_SCANNING,
		
		END_STATE_ASSOCIATING,
		END_STATE_RUNNING,
		
		CRD_STATE_RUNNING
	};

	uint8_t  m_State;
	uint32_t m_Error;
	uint32_t m_Version;
	uint16_t m_PanId;
	uint16_t m_LocalAddress;

	WUARTStream m_WUartArray[MAX_WUART];
	uint32_t m_WUartNum;
	
	union {
		struct {
			uint32_t ChannelsMask;
			uint8_t ScanDuration;
			MAC_MlmeScanType_e ScanType;
		} Scan;

		struct {
			uint8_t  LogicalChannel;
			uint8_t  BeaconOrder;
			uint8_t  SuperframeOrder;
			uint8_t  PanCoordinator;
			uint8_t  BatteryLifeExt;
			uint8_t  Realignment;
			//uint8_t  u8SecurityEnable;
			//uint32_T u32StarTime;
			//MAC_SecurityData_s  sCoordRealignSecurity;
			//MAC_SecurityData_s  sBeaconSecurity;
		} Coord;

		struct {
			uint8_t  LogicalChannel;
			uint8_t  Capability;
			uint8_t  CoordinatorAddressMode;
			uint8_t  CoordinatorAddress;
		} EndD;
	} m_Config;

	WUARTStream m_NullWUART;

public:
	JN_AN_1178_Protocol();

	uint16_t PanId() { return m_PanId; }

	void init(uint16_t panid);
	void startCoordinator(uint32_t u32ChannelstoScan=0x7FFFC00);
	void startEndDevice(uint32_t u32ChannelstoScan=0x7FFFC00);

protected:
	void confirmScanSuccess(uint8_t scantype, int scancount, uint32_t channelmask);
	void confirmScanFailed(uint8_t reason);
	void confirmAssociateSuccess(uint16_t u16ShortAddr);
	void confirmAssociateFailed(uint8_t reason);
	void indicateData(uint16_t u16SrcAddr, uint16_t u16DstAddr, uint8_t* au8Sdu, uint8_t u8SduLength);
	void indicateAssociate(uint32_t addrH, uint32_t addrL, uint8_t capability);

private:
	 uint8_t scannedCount();
	uint16_t scannedAddress(int netnum);
	uint16_t scannedPanId(int netnum);
	uint16_t scannedLogicalChannel(int netnum);
	 uint8_t scannedEnergyLevel(int netnum);

	void scan(uint8_t u8Scantype, uint32_t u32ChannelstoScan=0x7FFFC00, uint8_t u8ScanDuration=3);
	void data(uint16_t shortaddr, uint8_t* buf, uint8_t buflen);

	static void TickTimerISR(uint32_t u32Device, uint32_t u32ItemBitmap);
};

#endif /* JN_AN_1178_H_ */
