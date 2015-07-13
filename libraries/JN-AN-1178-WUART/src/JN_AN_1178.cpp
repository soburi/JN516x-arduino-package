/*
 * StateMachine.c
 *
 *  Created on: 2015/03/27
 *      Author: crs
 */

#include "JN_AN_1178.h"
#include "WUART.hpp"

#include <AppApi.h>
#include <AppHardwareApi.h>
#include <mac_sap.h>
#include <mac_pib.h>

#include <Arduino.h>
#include <stdlib.h>

//#define JNAN1178_DBG_PRINTF(...) DBG_PRINTF(__VA_ARGS__)
//#define JNAN1178_DBG_PRINTLN(...) { DBG_PRINTF(__VA_ARGS__); DBG_PRINTF("\r\n"); }
//#define JNAN1178_DBG_NL() DBG_PRINTF("\r\n")

#define JNAN1178_DBG_PRINTF(...)
#define JNAN1178_DBG_PRINTLN(...)
#define JNAN1178_DBG_NL()

#define JNAN1178_TRACE_FUNC 1

#ifdef JNAN1178_TRACE_FUNC
#define JNAN1178_DBG_FUNC_ENTER() JNAN1178_DBG_PRINTLN(__PRETTY_FUNCTION__)
#define JNAN1178_DBG_FUNC_PARAMD(x) JNAN1178_DBG_PRINTF(":" #x ":%d ", x)
#define JNAN1178_DBG_FUNC_PARAMX(x) JNAN1178_DBG_PRINTF(":" #x ":0x%x ", x)
#define JNAN1178_DBG_FUNC_PARAM(...) JNAN1178_DBG_PRINTF(__VA_ARGS__)
#define JNAN1178_DBG_FUNC_PARAMLN(...) JNAN1178_DBG_PRINTLN(__VA_ARGS__)
#define JNAN1178_DBG_FUNC_NL() JNAN1178_DBG_NL()
#else
#define JNAN1178_DBG_FUNC_ENTER()
#define JNAN1178_DBG_FUNC_PARAMD(x)
#define JNAN1178_DBG_FUNC_PARAMX(x)
#define JNAN1178_DBG_FUNC_PARAM(...)
#define JNAN1178_DBG_FUNC_PARAMLN(...)
#define JNAN1178_DBG_FUNC_NL()
#endif

JN_AN_1178_Protocol JN_AN_1178;

void onWUartAssociated(int id) __attribute__((weak));
void onWUartAssociated(int id)
{
	(void)id;
	JNAN1178_DBG_PRINTF("onWUartAssociated(%d)\r\n", id);
}


JN_AN_1178_Protocol::WUART_Accessor::WUART_Accessor(JN_AN_1178_Protocol& protocol)
	: m_Protocol(protocol)
{
}

WUARTStream& JN_AN_1178_Protocol::WUART_Accessor::operator()(int destaddr=0) const
{
	//JNAN1178_DBG_FUNC_ENTER();
	//JNAN1178_DBG_FUNC_PARAM("destaddr %d ", destaddr);
	//JNAN1178_DBG_FUNC_PARAM("NUM %d ", m_Protocol.m_WUartNum);
	//JNAN1178_DBG_FUNC_NL();

	for(uint32_t i=0; i<m_Protocol.m_WUartNum; i++)
	{
		//JNAN1178_DBG_PRINTF("%d\r\n", i);
		//JNAN1178_DBG_PRINTF("array %x\r\n",  m_Protocol.m_WUartArray);
		//JNAN1178_DBG_PRINTF("entry %x\r\n",  m_Protocol.m_WUartArray[i]);
		//JNAN1178_DBG_PRINTF("destination %d\r\n",  m_Protocol.m_WUartArray[i].destinationAddress() );

		if( m_Protocol.m_WUartArray[i].destinationAddress() == destaddr)
		{
			//JNAN1178_DBG_PRINTF("found %d\r\n", i);
			return m_Protocol.m_WUartArray[i];
		}
	}
	JNAN1178_DBG_PRINTF("not found\r\n");
	return m_Protocol.m_NullWUART;
}

WUARTStream& JN_AN_1178_Protocol::WUART_Accessor::operator[](int idx) const
{
	if(idx >= (int)m_Protocol.m_WUartNum) return m_Protocol.m_NullWUART;
	return m_Protocol.m_WUartArray[idx];
}




JN_AN_1178_Protocol::JN_AN_1178_Protocol()
	: WUART(*this), m_NullWUART()
{
	JNAN1178_DBG_PRINTF("m_WUartArray 0x%x\r\n", m_WUartArray);
	m_WUartNum = 0;

	/* Set invalid addresses */
	m_LocalAddress  = 0xFFFF;
}

void JN_AN_1178_Protocol::init(uint16_t u16Panid)
{
	JNAN1178_DBG_FUNC_ENTER();
	JNAN1178_DBG_FUNC_PARAMX(u16Panid);
	JNAN1178_DBG_FUNC_NL();

	IEEE802154::init(u16Panid);

	m_PanId = u16Panid;
	m_Config.Scan.ScanDuration = 3;
	m_Config.Scan.ScanType = MAC_MLME_SCAN_TYPE_ENERGY_DETECT;
	m_Config.Scan.ChannelsMask = 0;
	
	m_State   = STATE_INIT;
}

static JN_AN_1178_Protocol* This = 0;

void JN_AN_1178_Protocol::TickTimerISR(uint32_t u32Device, uint32_t u32ItemBitmap)
{
	(void)u32Device; (void)u32ItemBitmap;
	//JNAN1178_DBG_PRINTLN("TickTimerISR");
	for(uint32_t i=0; i<This->m_WUartNum; i++)
	{
		This->m_WUartArray[i].Tx();
	}
}


void JN_AN_1178_Protocol::startCoordinator(uint32_t u32ChannelstoScan)
{	
	JNAN1178_DBG_PRINTLN("pvAppApiGetMacHandle");
	void *pvMac;
	MAC_Pib_s *psPib;

	pvMac = pvAppApiGetMacHandle();
	MAC_vPibSetShortAddr(pvMac, 0);
	psPib = MAC_psPibGetHandle(pvMac);

	/* Allow nodes to associate */
	psPib->bAssociationPermit = 1;
	
	This = this;



	m_State = STATE_SCANNING;
	scan(MAC_MLME_SCAN_TYPE_ENERGY_DETECT, u32ChannelstoScan);
}

void JN_AN_1178_Protocol::startEndDevice(uint32_t u32ChannelstoScan)
{
	m_State = STATE_SCANNING;
	scan(MAC_MLME_SCAN_TYPE_ACTIVE, u32ChannelstoScan);
}

void JN_AN_1178_Protocol::scan(uint8_t u8ScanType, uint32_t u32ChannelstoScan, uint8_t u8ScanDuration)
{
	JNAN1178_DBG_FUNC_ENTER();
	JNAN1178_DBG_FUNC_PARAMD(u8ScanType);
	JNAN1178_DBG_FUNC_PARAMD(u32ChannelstoScan);
	JNAN1178_DBG_FUNC_NL();

	m_Config.Scan.ScanType = (MAC_MlmeScanType_e)u8ScanType;
	m_Config.Scan.ChannelsMask = u32ChannelstoScan;
	m_Config.Scan.ScanDuration = u8ScanDuration;

	requestScan(u8ScanType, u32ChannelstoScan, m_Config.Scan.ScanDuration);
}

void JN_AN_1178_Protocol::data(uint16_t shortaddr, uint8_t* buf, uint8_t buflen)
{
	JNAN1178_DBG_FUNC_ENTER();
	JNAN1178_DBG_FUNC_PARAM(": buflen:%d", buflen);
	JNAN1178_DBG_FUNC_PARAM(": local:%x", m_LocalAddress);
	JNAN1178_DBG_FUNC_PARAM(": remote:%x", shortaddr);
	JNAN1178_DBG_FUNC_NL();

	//JNAN1178_DBG_PRINTLN(__PRETTY_FUNCTION__);
	//JNAN1178_DBG_PRINTLN("%x\r\n", shortaddr);

	requestData(0, m_PanId, m_LocalAddress, shortaddr, buf, buflen, 0U);
}

uint8_t JN_AN_1178_Protocol::scannedCount()
{
	MAC_MlmeDcfmInd_s* psDcfmInd = &MlmeBuffer();
	return psDcfmInd->uParam.sDcfmScan.u8ResultListSize;
}

uint16_t JN_AN_1178_Protocol::scannedAddress(int netnum)
{
	MAC_MlmeDcfmInd_s* psDcfmInd = &MlmeBuffer();
	MAC_PanDescr_s* psPanDesc = &psDcfmInd->uParam.sDcfmScan.uList.asPanDescr[netnum];
	return psPanDesc->sCoord.uAddr.u16Short;
}

uint16_t JN_AN_1178_Protocol::scannedPanId(int netnum)
{
	MAC_MlmeDcfmInd_s* psDcfmInd = &MlmeBuffer();
	MAC_PanDescr_s* psPanDesc = &psDcfmInd->uParam.sDcfmScan.uList.asPanDescr[netnum];
	return psPanDesc->sCoord.u16PanId;
}

uint16_t JN_AN_1178_Protocol::scannedLogicalChannel(int netnum)
{
	MAC_MlmeDcfmInd_s* psDcfmInd = &MlmeBuffer();
	MAC_PanDescr_s* psPanDesc = &psDcfmInd->uParam.sDcfmScan.uList.asPanDescr[netnum];
	return psPanDesc->u8LogicalChan;
}

uint8_t JN_AN_1178_Protocol::scannedEnergyLevel(int netnum)
{
	MAC_MlmeDcfmInd_s* psDcfmInd = &MlmeBuffer();
	uint8* energydetect = psDcfmInd->uParam.sDcfmScan.uList.au8EnergyDetect;
	return energydetect[netnum];
}

void JN_AN_1178_Protocol::confirmScanSuccess(uint8_t scantype, int scancount, uint32_t channelmask) {
	(void)channelmask;
	JNAN1178_DBG_FUNC_ENTER();
	JNAN1178_DBG_FUNC_PARAM(": scantype:%d", scantype);
	JNAN1178_DBG_FUNC_PARAM(": scancount:%d", scancount);
	JNAN1178_DBG_FUNC_PARAM(": channelmask:%d", channelmask);
	JNAN1178_DBG_FUNC_PARAM(": ctx->NUM_Conn:%d", m_WUartNum);
	JNAN1178_DBG_FUNC_NL();
	
	if (m_State != STATE_SCANNING)
	{
		return;
	}


	if(scantype == MAC_MLME_SCAN_TYPE_ACTIVE)
	{
		for (int i=0; i<scancount; i++)
		{
			JNAN1178_DBG_PRINTLN("count:%d panid:%x", i, scannedPanId(i) );

			if (m_PanId != scannedPanId(i) )
			{
				JNAN1178_DBG_PRINTLN(" : panid:%d", scannedPanId(i) );
				continue;
			}

			m_Config.EndD.CoordinatorAddress = scannedAddress(i);
			m_Config.EndD.LogicalChannel = scannedLogicalChannel(i);

			/* Matched so start to synchronise and associate */
			m_State = END_STATE_ASSOCIATING;


			JNAN1178_DBG_PRINTF("requestAssociate");
			JNAN1178_DBG_PRINTF(": PanId:0x%x" ,m_PanId);
			JNAN1178_DBG_PRINTF(": LogicalChannel:", m_Config.EndD.LogicalChannel);
			JNAN1178_DBG_PRINTLN(": CoordinatorAddress:", m_Config.EndD.CoordinatorAddress);

			requestAssociate(m_PanId, m_Config.EndD.LogicalChannel, m_Config.EndD.CoordinatorAddress);

			return;
		}
	}
	else if(scantype == MAC_MLME_SCAN_TYPE_ENERGY_DETECT)
	{
		uint8_t minEnergy =  scannedEnergyLevel(0);
		m_Config.Coord.LogicalChannel = 11;
		for(uint8_t i=0; i< scannedCount(); i++)
		{
			uint8_t energy =  scannedEnergyLevel(i);
			JNAN1178_DBG_PRINTLN("energy %d", energy);
			if (energy < minEnergy)
			{
				minEnergy = energy;
				m_Config.Coord.LogicalChannel = i + 11;
			}
		}
		JNAN1178_DBG_PRINTLN("%d", m_Config.Coord.LogicalChannel);

		m_Config.Coord.BeaconOrder = 0x0F;
		m_Config.Coord.SuperframeOrder = 0x0F;
		m_Config.Coord.PanCoordinator = true;
		m_Config.Coord.BatteryLifeExt = false;
		m_Config.Coord.Realignment = false;
		m_LocalAddress = 0;

		JNAN1178_DBG_PRINTLN("%d", m_Config.Coord.LogicalChannel);

		JNAN1178_DBG_PRINTLN("requestStart:");
		requestStart(m_PanId, m_Config.Coord.LogicalChannel, m_Config.Coord.BeaconOrder,
				m_Config.Coord.SuperframeOrder, m_Config.Coord.PanCoordinator, m_Config.Coord.BatteryLifeExt,
				m_Config.Coord.Realignment, 0);
		
		This = this;
		vAHI_Timer4RegisterCallback(JN_AN_1178_Protocol::TickTimerISR);
		vAHI_TimerDIOControl(E_AHI_TIMER_4, false);
		vAHI_TimerEnable(E_AHI_TIMER_4, 8, false, true, false);
		vAHI_TimerStartRepeat(E_AHI_TIMER_4, 0, 625);

		m_State = CRD_STATE_RUNNING;
		return;
	}

	/* No potential parent has been found, simply scan again */
	m_State = STATE_SCANNING;
	scan(MAC_MLME_SCAN_TYPE_ACTIVE, m_Config.Scan.ChannelsMask);
}

void JN_AN_1178_Protocol::confirmScanFailed(uint8_t reason)
{
	(void)reason;
	/* No potential parent has been found, simply scan again */
	m_State = STATE_SCANNING;
	scan(MAC_MLME_SCAN_TYPE_ACTIVE, m_Config.Scan.ChannelsMask);
}

void JN_AN_1178_Protocol::confirmAssociateSuccess(uint16_t u16ShortAddr)
{
	JNAN1178_DBG_FUNC_ENTER();
	JNAN1178_DBG_FUNC_PARAM(": u16ShortAddr:0x%x", u16ShortAddr);
	JNAN1178_DBG_FUNC_NL();

	//WUARTStream** tmp = (WUARTStream**)realloc(m_WUartArray, sizeof(WUARTStream*) * m_WUartNum);
	//m_WUartArray = tmp;
	//JNAN1178_DBG_PRINTF("m_WUartArray 0x%x %d\r\n", m_WUartArray, sizeof(WUARTStream*) * m_WUartNum);
	//m_WUartArray[m_WUartNum] = new WUARTStream(*ro, false);
	if (m_State != END_STATE_ASSOCIATING)
	{
		return;
	}

        /* We are now in the running state */
	m_State = END_STATE_RUNNING;
	m_WUartArray[m_WUartNum].Init(WUARTStream::TYPE_ENDD, m_Config.EndD.CoordinatorAddress, this);

	m_WUartNum += 1;

	m_LocalAddress = u16ShortAddr;
	onWUartAssociated(m_Config.EndD.CoordinatorAddress);

	This = this;
	vAHI_Timer4RegisterCallback(JN_AN_1178_Protocol::TickTimerISR);
	vAHI_TimerDIOControl(E_AHI_TIMER_4, false);
	vAHI_TimerEnable(E_AHI_TIMER_4, 8, false, true, false);
	vAHI_TimerStartRepeat(E_AHI_TIMER_4, 0, 625);

	JNAN1178_DBG_PRINTLN(": ctx->AssociateAddress:0x%x", m_LocalAddress);
}

void JN_AN_1178_Protocol::confirmAssociateFailed(uint8_t reason)
{
    (void)reason;
    if (m_State != END_STATE_ASSOCIATING)
    {
        return;
    }

    {
        /* Try, try again */
	m_State = STATE_SCANNING;
	scan(MAC_MLME_SCAN_TYPE_ACTIVE, m_Config.Scan.ChannelsMask);
    }
}

void JN_AN_1178_Protocol::indicateData(uint16_t u16SrcAddr, uint16_t u16DstAddr, uint8_t* au8Sdu, uint8_t u8SduLength)
{
	(void)u16DstAddr;
	JNAN1178_DBG_FUNC_ENTER();
	//JNAN1178_DBG_PRINTLN(__PRETTY_FUNCTION__);
	//JNAN1178_DBG_PRINTF(": u16SrcAddr:0x%x", u16SrcAddr);
	//JNAN1178_DBG_PRINTLN(": u16DstAddr:0x%x", u16DstAddr);

	for (uint8_t i=0; i<m_WUartNum; i++)
	{
		//JNAN1178_DBG_PRINTLN(": destinationAddress :0x%x", m_WUartArray[i].destinationAddress() );
		if(m_WUartArray[i].destinationAddress() == u16SrcAddr)
		{
			m_WUartArray[i].Rx(au8Sdu, u8SduLength);
		}
	}
}

void JN_AN_1178_Protocol::indicateAssociate(uint32_t addrH, uint32_t addrL, uint8_t capability)
{
	JNAN1178_DBG_FUNC_ENTER();

	uint16 u16ShortAddress = 0xffff;
	uint8 u8AssocStatus = 2;

	/* Check that the device only wants to use a short address */
	if (capability & 0x80)
	{
		//vSerialQ_AddString(TX_QUEUE, "uParam.sIndAssociate.u8Capability\r\n");
		//vUART_StartTx();
		if (m_WUartNum < MAX_WUART)
		{
			//vSerialQ_AddString(TX_QUEUE, "sCrdData.sNode.u8AssociatedNodes\r\n");
			/* Allocate short address as next in list */
			u16ShortAddress = 1 + m_WUartNum;
			u8AssocStatus = 0;
			/* Store details for future use */
			//sCrdData.sNode.asAssocNodes[1 + m_WUartNum].u16ShortAddr = u16ShortAddress;
			//sCrdData.sNode.u8AssociatedNodes++;
			//vRadio_SetAddressRemote(u16ShortAddress);
			m_WUartArray[m_WUartNum].Init(WUARTStream::TYPE_COORDINATOR, u16ShortAddress, this);
			//m_WUartArray[m_WUartNum].setDestinationAddress(u16ShortAddress);
			onWUartAssociated(u16ShortAddress);
			m_WUartNum++;
		}
	}

	reqponseAssociate(addrH, addrL, u16ShortAddress, u8AssocStatus, false);
}
