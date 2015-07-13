/*
 * WPAN.cpp
 *
 *  Created on: 2015/03/23
 *      Author: crs
 */

#include "IEEE802154_func.inc"

#include <mac_sap.h>
#include <mac_pib.h>
#include <AppApi.h>

#include <Arduino.h>

//#define IEEE802154_DEBUG

#define IEEE802154_DBG_PRINTF(...) DBG_PRINTF(__VA_ARGS__)
#define IEEE802154_DBG_PRINTLN(...) { DBG_PRINTF(__VA_ARGS__); DBG_PRINTF("\r\n"); }
#define IEEE802154_DBG_NL() DBG_PRINTF("\r\n")

//#define IEEE802154_TRACE_FUNC 1

#ifdef IEEE802154_TRACE_FUNC
#define IEEE802154_DBG_FUNC_ENTER() IEEE802154_DBG_PRINTLN(__PRETTY_FUNCTION__)
#define IEEE802154_DBG_FUNC_PARAM(...) IEEE802154_DBG_PRINTF(__VA_ARGS__)
#define IEEE802154_DBG_FUNC_PARAMLN(...) IEEE802154_DBG_PRINTLN(__VA_ARGS__)
#define IEEE802154_DBG_FUNC_NL() IEEE802154_DBG_NL()
#else
#define IEEE802154_DBG_FUNC_ENTER()
#define IEEE802154_DBG_FUNC_PARAM(...)
#define IEEE802154_DBG_FUNC_PARAMLN(...)
#define IEEE802154_DBG_FUNC_NL()
#endif

void IEEE802154_MLME_associate(uint16_t panId, uint8_t logicalChannel, uint16_t coordaddr)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_MlmeReqRsp_s  sReqRsp;
	MAC_MlmeSyncCfm_s sMlmeSyncCfm;

	sReqRsp.u8Type = MAC_MLME_REQ_ASSOCIATE;
	sReqRsp.u8ParamLength = sizeof(MAC_MlmeReqAssociate_s);

	sReqRsp.uParam.sReqAssociate.u8Capability = 0x80; /* We want short address, other features off */
	sReqRsp.uParam.sReqAssociate.u8SecurityEnable = FALSE;
	sReqRsp.uParam.sReqAssociate.u8LogicalChan = logicalChannel;
	sReqRsp.uParam.sReqAssociate.sCoord.u8AddrMode = 2;
	sReqRsp.uParam.sReqAssociate.sCoord.u16PanId = panId;
	sReqRsp.uParam.sReqAssociate.sCoord.uAddr.u16Short = coordaddr;

	IEEE802154_DBG_PRINTLN("sReqRsp.u8Type %04x", sReqRsp.u8Type);
	IEEE802154_DBG_PRINTLN("sReqRsp.u8ParamLength %04x", sReqRsp.u8ParamLength);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqAssociate.u8LogicalChan %04x", sReqRsp.uParam.sReqAssociate.u8LogicalChan);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqAssociate.u8Capability %04x", sReqRsp.uParam.sReqAssociate.u8Capability);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqAssociate.sCoord.u8AddrMode %04x", sReqRsp.uParam.sReqAssociate.sCoord.u8AddrMode);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqAssociate.sCoord.u16PanId %04x", sReqRsp.uParam.sReqAssociate.sCoord.u16PanId);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqAssociate.sCoord.uAddr.u16Short %04x", sReqRsp.uParam.sReqAssociate.sCoord.uAddr.u16Short);


	vAppApiMlmeRequest(&sReqRsp, &sMlmeSyncCfm);
}


void IEEE802154_MLME_disassociate(uint16_t u16Address, uint8_t u8Reason)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_MlmeReqRsp_s  sMlmeReqRsp;
	MAC_MlmeSyncCfm_s sMlmeSyncCfm;

	sMlmeReqRsp.u8Type = MAC_MLME_REQ_DISASSOCIATE;
	sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqDisassociate_s);
	sMlmeReqRsp.uParam.sReqDisassociate.sAddr.u8AddrMode = 2;
	sMlmeReqRsp.uParam.sReqDisassociate.sAddr.uAddr.u16Short = u16Address;
	sMlmeReqRsp.uParam.sReqDisassociate.u8Reason = u8Reason;
	sMlmeReqRsp.uParam.sReqDisassociate.u8SecurityEnable = FALSE;

	vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);
}


void IEEE802154_MLME_get(uint8_t pibAttribute, uint8_t pibAttributeIndex)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_MlmeReqRsp_s  sMlmeReqRsp;
	MAC_MlmeSyncCfm_s sMlmeSyncCfm;

	sMlmeReqRsp.u8Type = MAC_MLME_REQ_GET;
	sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqGet_s);
	sMlmeReqRsp.uParam.sReqGet.u8PibAttribute = pibAttribute;
	sMlmeReqRsp.uParam.sReqGet.u8PibAttributeIndex = pibAttributeIndex;

	vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);

	IEEE802154_DBG_PRINTLN(": u8Status:%d", sMlmeSyncCfm.u8Status);
}


void IEEE802154_MLME_gts(uint8_t characteristics, uint8_t securityEnable)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_MlmeReqRsp_s  sMlmeReqRsp;
	MAC_MlmeSyncCfm_s sMlmeSyncCfm;

	sMlmeReqRsp.u8Type = MAC_MLME_REQ_GTS;
	sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqGts_s);
	sMlmeReqRsp.uParam.sReqGts.u8Characteristics = characteristics;
	sMlmeReqRsp.uParam.sReqGts.u8SecurityEnable = securityEnable;

	vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);
}



void IEEE802154_MLME_reset(uint8_t u8SetDefaultPib)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_MlmeReqRsp_s sMlmeReqRsp;
	MAC_MlmeSyncCfm_s sMlmeSyncCfm;

	sMlmeReqRsp.u8Type = MAC_MLME_REQ_RESET;
	sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqReset_s);
	sMlmeReqRsp.uParam.sReqReset.u8SetDefaultPib = u8SetDefaultPib;

	vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);

	IEEE802154_DBG_PRINTLN(": u8Status:%d", sMlmeSyncCfm.u8Status);
}

void IEEE802154_MLME_rx_enable(uint32_t u32RxOnTime, uint32_t u32RxOnDuration, uint8_t  u8DeferPermit)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_MlmeReqRsp_s sMlmeReqRsp;
	MAC_MlmeSyncCfm_s sMlmeSyncCfm;

	sMlmeReqRsp.u8Type = MAC_MLME_REQ_RX_ENABLE;
	sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqRxEnable_s);
	sMlmeReqRsp.uParam.sReqRxEnable.u32RxOnTime = u32RxOnTime;
	sMlmeReqRsp.uParam.sReqRxEnable.u32RxOnDuration = u32RxOnDuration;
	sMlmeReqRsp.uParam.sReqRxEnable.u8DeferPermit = u8DeferPermit;
	vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);
	/* Handle synchronous confirm */
	if (sMlmeSyncCfm.u8Status != MAC_MLME_CFM_DEFERRED)
	{
		/* Unexpected result: scan request should result in a deferred
		confirmation (i.e. we will receive it later) */
	}
}

void IEEE802154_MLME_scan(uint8_t scantype, uint32_t u32ChannelstoScan, uint8_t u8ScanDuration)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_MlmeReqRsp_s sReqRsp;
	MAC_MlmeSyncCfm_s sMlmeSyncCfm;

	sReqRsp.u8Type = MAC_MLME_REQ_SCAN;
	sReqRsp.u8ParamLength = sizeof(MAC_MlmeReqScan_s);
	sReqRsp.uParam.sReqScan.u8ScanType = scantype;
	sReqRsp.uParam.sReqScan.u32ScanChannels = u32ChannelstoScan;
	sReqRsp.uParam.sReqScan.u8ScanDuration = u8ScanDuration;

	IEEE802154_DBG_PRINTLN("sReqRsp.u8Type %04x", sReqRsp.u8Type);
	IEEE802154_DBG_PRINTLN("sReqRsp.u8ParamLength %04x", sReqRsp.u8ParamLength);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqScan.u8ScanType %04x", sReqRsp.uParam.sReqScan.u8ScanType);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqScan.u32ScanChannels %04x", sReqRsp.uParam.sReqScan.u32ScanChannels);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqScan.u8ScanDuration %04x", sReqRsp.uParam.sReqScan.u8ScanDuration);

	vAppApiMlmeRequest(&sReqRsp, &sMlmeSyncCfm);
	/* Handle synchronous confirm */
	if (sMlmeSyncCfm.u8Status != MAC_MLME_CFM_DEFERRED)
	{
		/* Unexpected result: scan request should result in a deferred
		confirmation (i.e. we will receive it later) */
	}
}

union WPAN_MLME_SetValue {
	uint64_t u64;
	struct {
		uint8_t* au8;
		uint32_t  len;
	} buf;
};

static inline uint32_t H32(uint64_t v) {
	return ( (v & 0xFFFFFFFF00000000ull) >> 32);
}

static inline uint32_t L32(uint64_t v) {
	return   (v & 0x00000000FFFFFFFFull);
}

static void WPAN_MLME_set_impl(uint8_t pibAttribute, union WPAN_MLME_SetValue value, uint8_t pibAttributeIndex)
{
	MAC_MlmeReqRsp_s sMlmeReqRsp;
	MAC_MlmeSyncCfm_s sMlmeSyncCfm;

	MAC_MlmeReqSet_s* psReqSet = &sMlmeReqRsp.uParam.sReqSet;

	IEEE802154_DBG_FUNC_ENTER();

	sMlmeReqRsp.u8Type = MAC_MLME_REQ_SET;
	sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqSet_s);
	psReqSet->u8PibAttribute = pibAttribute;
	psReqSet->u8PibAttributeIndex = pibAttributeIndex;

	switch(pibAttribute) {
		case MAC_PIB_ATTR_ACK_WAIT_DURATION:
			psReqSet->uPibAttributeValue.u8AckWaitDuration = value.u64;
			break;
		case MAC_PIB_ATTR_ASSOCIATION_PERMIT:
			psReqSet->uPibAttributeValue.u8AssociationPermit = value.u64;
			break;
		case MAC_PIB_ATTR_AUTO_REQUEST:
			psReqSet->uPibAttributeValue.u8AutoRequest = value.u64;
			break;
		case MAC_PIB_ATTR_BATT_LIFE_EXT:
			psReqSet->uPibAttributeValue.u8BattLifeExt = value.u64;
			break;
		case MAC_PIB_ATTR_BATT_LIFE_EXT_PERIODS:
			psReqSet->uPibAttributeValue.u8BattLifeExtPeriods = value.u64;
			break;
		case MAC_PIB_ATTR_BEACON_PAYLOAD:
			for(uint32_t i=0; i<(value.buf.len < MAC_MAX_BEACON_PAYLOAD_LEN ? value.buf.len : MAC_MAX_BEACON_PAYLOAD_LEN); i++) {
				psReqSet->uPibAttributeValue.au8BeaconPayload[i] = value.buf.au8[i];
			}
			break;
		case MAC_PIB_ATTR_BEACON_PAYLOAD_LENGTH:
			psReqSet->uPibAttributeValue.u8BeaconPayloadLength = value.u64;
			break;
		case MAC_PIB_ATTR_BEACON_ORDER:
			psReqSet->uPibAttributeValue.u8BeaconOrder = value.u64;
			break;
		case MAC_PIB_ATTR_BEACON_TX_TIME:
			psReqSet->uPibAttributeValue.u32BeaconTxTime = value.u64;
			break;
		case MAC_PIB_ATTR_BSN:
			psReqSet->uPibAttributeValue.u8Bsn = value.u64;
			break;
		case MAC_PIB_ATTR_COORD_EXTENDED_ADDRESS:
			psReqSet->uPibAttributeValue.sCoordExtAddr.u32L = L32(value.u64);
			psReqSet->uPibAttributeValue.sCoordExtAddr.u32H = H32(value.u64);
			break;
		case MAC_PIB_ATTR_COORD_SHORT_ADDRESS:
			psReqSet->uPibAttributeValue.u16CoordShortAddr = value.u64;
			break;
		case MAC_PIB_ATTR_DSN:
			psReqSet->uPibAttributeValue.u8Dsn = value.u64;
			break;
		case MAC_PIB_ATTR_GTS_PERMIT:
			psReqSet->uPibAttributeValue.u8GtsPermit = value.u64;
			break;
		case MAC_PIB_ATTR_MAX_CSMA_BACKOFFS:
			psReqSet->uPibAttributeValue.u8MaxCsmaBackoffs = value.u64;
			break;
		case MAC_PIB_ATTR_MIN_BE:
			psReqSet->uPibAttributeValue.u8MinBe = value.u64;
			break;
		case MAC_PIB_ATTR_PAN_ID:
			psReqSet->uPibAttributeValue.u16PanId = value.u64;
			break;
		case MAC_PIB_ATTR_PROMISCUOUS_MODE:
			psReqSet->uPibAttributeValue.u8PromiscuousMode = value.u64;
			break;
		case MAC_PIB_ATTR_RX_ON_WHEN_IDLE:
			psReqSet->uPibAttributeValue.u8RxOnWhenIdle = value.u64;
			break;
		case MAC_PIB_ATTR_SHORT_ADDRESS:
			psReqSet->uPibAttributeValue.u16ShortAddr = value.u64;
			break;
		case MAC_PIB_ATTR_SUPERFRAME_ORDER:
			psReqSet->uPibAttributeValue.u8SuperframeOrder = value.u64;
			break;
		case MAC_PIB_ATTR_TRANSACTION_PERSISTENCE_TIME:
			psReqSet->uPibAttributeValue.u16TransactionPersistenceTime = value.u64;
			break;
		case MAC_PIB_ATTR_MAX_FRAME_TOTAL_WAIT_TIME:
		    psReqSet->uPibAttributeValue.u16MaxTotalFrameTxTime = value.u64;
			break;
		case MAC_PIB_ATTR_MAX_FRAME_RETRIES:
			psReqSet->uPibAttributeValue.u8MaxFrameRetries = value.u64;
			break;
		case MAC_PIB_ATTR_RESPONSE_WAIT_TIME:
			psReqSet->uPibAttributeValue.u8ResponseWaitTime = value.u64;
			break;
		/* 2006 Security attributes */
		case MAC_PIB_ATTR_SECURITY_ENABLED:
			psReqSet->uPibAttributeValue.bMacSecurityEnabled = value.u64;
			break;
		/* Security attributes */
		case MAC_PIB_ATTR_ACL_ENTRY_DESCRIPTOR_SET:
//TODO			psReqSet->uPibAttributeValue.sAclEntry
			break;
		case MAC_PIB_ATTR_ACL_ENTRY_DESCRIPTOR_SET_SIZE:
			psReqSet->uPibAttributeValue.u8AclEntryDescriptorSetSize = value.u64;
			break;
		case MAC_PIB_ATTR_DEFAULT_SECURITY:
			psReqSet->uPibAttributeValue.u8DefaultSecurity = value.u64;
			break;
		case MAC_PIB_ATTR_ACL_DEFAULT_SECURITY_MATERIAL_LENGTH:
			psReqSet->uPibAttributeValue.u8AclDefaultSecurityMaterialLength = value.u64;
			break;
		case MAC_PIB_ATTR_DEFAULT_SECURITY_MATERIAL:
			for(uint32_t i=0; i<(value.buf.len < MAC_MAX_SECURITY_MATERIAL_LEN ? value.buf.len : MAC_MAX_SECURITY_MATERIAL_LEN); i++) {
				psReqSet->uPibAttributeValue.au8DefaultSecurityMaterial[i] = value.buf.au8[i];
			}
			break;
		case MAC_PIB_ATTR_DEFAULT_SECURITY_SUITE:
			psReqSet->uPibAttributeValue.u8DefaultSecuritySuite = value.u64;
			break;
		case MAC_PIB_ATTR_SECURITY_MODE:
			psReqSet->uPibAttributeValue.u8SecurityMode = value.u64;
			break;
		case MAC_PIB_ATTR_MACFRAMECOUNTER:
			break;
		default:
			break;
	}

	vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);

	IEEE802154_DBG_PRINTLN("WPAN_MLME_set: u8Status:%d", sMlmeSyncCfm.u8Status);
}


void IEEE802154_MLME_set(uint8_t pibAttribute, uint64_t u64Value, uint8_t pibAttributeIndex)
{
	IEEE802154_DBG_FUNC_ENTER();

	union WPAN_MLME_SetValue value;
	value.u64 = u64Value;

	WPAN_MLME_set_impl(pibAttribute, value, pibAttributeIndex);
}

void IEEE802154_MLME_set_buf(uint8_t pibAttribute, uint8_t* au8Buf, uint32_t buflen, uint8_t pibAttributeIndex)
{
	IEEE802154_DBG_FUNC_ENTER();

	union WPAN_MLME_SetValue value;
	value.buf.au8 = au8Buf;
	value.buf.len = buflen;

	WPAN_MLME_set_impl(pibAttribute, value, pibAttributeIndex);
}

void IEEE802154_MLME_start(uint16_t u16PanId, uint8_t  u8Channel, uint8_t  u8BeaconOrder,
		uint8_t  u8SuperframeOrder, uint8_t  u8PanCoordinator, uint8_t  u8BatteryLifeExt,
		uint8_t  u8Realignment, uint32_t u32StarTime)
{
	IEEE802154_DBG_FUNC_ENTER();

	/* Structures used to hold data for MLME request and response */
	MAC_MlmeReqRsp_s sMlmeReqRsp;
	MAC_MlmeSyncCfm_s sMlmeSyncCfm;

	/* Start beacons */
	sMlmeReqRsp.u8Type = MAC_MLME_REQ_START;
	sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqStart_s);
	sMlmeReqRsp.uParam.sReqStart.u16PanId = u16PanId;
	sMlmeReqRsp.uParam.sReqStart.u8Channel = u8Channel;
	sMlmeReqRsp.uParam.sReqStart.u8BeaconOrder = u8BeaconOrder;
	sMlmeReqRsp.uParam.sReqStart.u8SuperframeOrder = u8SuperframeOrder;
	sMlmeReqRsp.uParam.sReqStart.u8PanCoordinator = u8PanCoordinator;
	sMlmeReqRsp.uParam.sReqStart.u8BatteryLifeExt = u8BatteryLifeExt;
	sMlmeReqRsp.uParam.sReqStart.u8Realignment = u8Realignment;
	sMlmeReqRsp.uParam.sReqStart.u8SecurityEnable = false;
	sMlmeReqRsp.uParam.sReqStart.u32StarTime = u32StarTime;

	IEEE802154_DBG_PRINTLN("u16PanId %x", sMlmeReqRsp.uParam.sReqStart.u16PanId);
	IEEE802154_DBG_PRINTLN("u8Channel %x", sMlmeReqRsp.uParam.sReqStart.u8Channel);
	IEEE802154_DBG_PRINTLN("u8BeaconOrder %x", sMlmeReqRsp.uParam.sReqStart.u8BeaconOrder);
	IEEE802154_DBG_PRINTLN("u8SuperframeOrder %x", sMlmeReqRsp.uParam.sReqStart.u8SuperframeOrder);
	IEEE802154_DBG_PRINTLN("u8PanCoordinator %x", sMlmeReqRsp.uParam.sReqStart.u8PanCoordinator);
	IEEE802154_DBG_PRINTLN("u8BatteryLifeExt %x", sMlmeReqRsp.uParam.sReqStart.u8BatteryLifeExt);
	IEEE802154_DBG_PRINTLN("u8Realignment %x", sMlmeReqRsp.uParam.sReqStart.u8Realignment);

	vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);

	IEEE802154_DBG_PRINTLN(": u8Status:%d", sMlmeSyncCfm.u8Status);
	IEEE802154_DBG_PRINTLN(": sCfmStart.u8Status:%d", sMlmeSyncCfm.uParam.sCfmStart.u8Status);

}

void IEEE802154_MLME_sync(uint8_t u8Channel, uint8_t u8TrackBeacon)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_MlmeReqRsp_s sMlmeReqRsp;
	MAC_MlmeSyncCfm_s sMlmeSyncCfm;

	sMlmeReqRsp.u8Type = MAC_MLME_REQ_SYNC;
	sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqSync_s);
	sMlmeReqRsp.uParam.sReqSync.u8Channel = u8Channel;
	sMlmeReqRsp.uParam.sReqSync.u8TrackBeacon = u8TrackBeacon;

	vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);
	IEEE802154_DBG_PRINTLN(": u8Status:%d", sMlmeSyncCfm.u8Status);
}

void IEEE802154_MLME_poll(uint16_t u16Addr)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_MlmeReqRsp_s sMlmeReqRsp;
	MAC_MlmeSyncCfm_s sMlmeSyncCfm;

	sMlmeReqRsp.u8Type = MAC_MLME_REQ_POLL;
	sMlmeReqRsp.u8ParamLength = sizeof(MAC_MlmeReqPoll_s);
	sMlmeReqRsp.uParam.sReqPoll.sCoord.u8AddrMode = 2;
	sMlmeReqRsp.uParam.sReqPoll.sCoord.uAddr.u16Short = u16Addr;
	sMlmeReqRsp.uParam.sReqPoll.u8SecurityEnable = FALSE;
	vAppApiMlmeRequest(&sMlmeReqRsp, &sMlmeSyncCfm);
	/* Handle synchronous confirm */
	if (sMlmeSyncCfm.u8Status != MAC_MLME_CFM_DEFERRED)
	{
		/* Unexpected result: scan request should result in a deferred
		confirmation (i.e. we will receive it later) */
	}
}

static void IEEE802154_MCPS_data_impl(uint8_t u8Ex, uint8_t u8Handle, uint8_t u8Channel, uint16_t u16Panid, uint16_t u16AddressLocal, uint16_t u16AddressRemote, uint8_t *pu8Data, uint8_t u8Length, uint8_t u8TxOptions)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_McpsReqRsp_s    sReqRsp;		/* Request/respsonse structure */
	MAC_McpsSyncCfm_s   sSyncCfm;		/* Synchronous confirmation structure */
	MAC_TxFrameData_s *psTxFrame;		/* Transmit frame pointer */

	if(pu8Data == NULL) {
		return;
	}

	sReqRsp.u8Type = (u8Ex ? MAC_MCPS_REQ_DATA_EXTENDED : MAC_MCPS_REQ_DATA);
	sReqRsp.u8ParamLength = sizeof(MAC_McpsReqData_s);
	sReqRsp.uParam.sReqData.u8Channel = u8Channel;
	sReqRsp.uParam.sReqData.u8Handle = u8Handle;

	psTxFrame = &sReqRsp.uParam.sReqData.sFrame;
	psTxFrame->sSrcAddr.u8AddrMode = 2;
	psTxFrame->sSrcAddr.u16PanId = u16Panid;
	psTxFrame->sSrcAddr.uAddr.u16Short = u16AddressLocal;
	psTxFrame->sDstAddr.u8AddrMode = 2;
	psTxFrame->sDstAddr.u16PanId = u16Panid;
	psTxFrame->sDstAddr.uAddr.u16Short = u16AddressRemote;
	psTxFrame->u8TxOptions = u8TxOptions;

	for (psTxFrame->u8SduLength = 0;
		 psTxFrame->u8SduLength < (u8Length <= MAC_MAX_DATA_PAYLOAD_LEN ? u8Length : MAC_MAX_DATA_PAYLOAD_LEN);
		 psTxFrame->u8SduLength++)
	{
		psTxFrame->au8Sdu[psTxFrame->u8SduLength] = pu8Data[psTxFrame->u8SduLength];
	} 
	
	/* Request transmit */
	vAppApiMcpsRequest(&sReqRsp, &sSyncCfm);
/*
	IEEE802154_DBG_PRINTLN("sReqRsp.u8Type %04x", sReqRsp.u8Type);
	IEEE802154_DBG_PRINTLN("sReqRsp.u8ParamLength %04x", sReqRsp.u8ParamLength);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqData.u8Channel %04x", sReqRsp.uParam.sReqData.u8Channel);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqData.u8Handle %04x", sReqRsp.uParam.sReqData.u8Handle);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqData.sFrame.sDstAddr.uAddr.u16Short %04x", sReqRsp.uParam.sReqData.sFrame.sDstAddr.uAddr.u16Short);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqData.sFrame.sSrcAddr.uAddr.u16Short %04x", sReqRsp.uParam.sReqData.sFrame.sSrcAddr.uAddr.u16Short);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqData.sFrame.u8SduLength %04x", sReqRsp.uParam.sReqData.sFrame.u8SduLength);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sReqData.sFrame.u8TxOptions %04x", sReqRsp.uParam.sReqData.sFrame.u8TxOptions);
*/
}

void IEEE802154_MCPS_data(uint8_t u8Handle, uint16_t u16Panid, uint16_t u16AddressLocal, uint16_t u16AddressRemote, uint8_t *pu8Data, uint8_t u8Length, uint8_t u8TxOptions)
{
	IEEE802154_MCPS_data_impl(0, u8Handle,         0, u16Panid, u16AddressLocal, u16AddressRemote, pu8Data, u8Length, u8TxOptions);
}
void IEEE802154_MCPS_data_ex(uint8_t u8Handle, uint8_t u8Channel, uint16_t u16Panid, uint16_t u16AddressLocal, uint16_t u16AddressRemote, uint8_t *pu8Data, uint8_t u8Length, uint8_t u8TxOptions)
{
	IEEE802154_MCPS_data_impl(1, u8Handle, u8Channel, u16Panid, u16AddressLocal, u16AddressRemote, pu8Data, u8Length, u8TxOptions);
}

void IEEE802154_MLME_resp_associate(uint32_t addrH, uint32_t addrL, uint16_t u16ShortAddress, uint8_t u8AssocStatus, bool bSecurity)
{
	MAC_MlmeReqRsp_s   sReqRsp;
	MAC_MlmeSyncCfm_s  sMlmeSyncCfm;

	/* Create association response */
	sReqRsp.u8Type = MAC_MLME_RSP_ASSOCIATE;
	sReqRsp.u8ParamLength = sizeof(MAC_MlmeRspAssociate_s);
	sReqRsp.uParam.sRspAssociate.sDeviceAddr.u32H = addrH;
	sReqRsp.uParam.sRspAssociate.sDeviceAddr.u32L = addrL;
	sReqRsp.uParam.sRspAssociate.u16AssocShortAddr = u16ShortAddress;
	sReqRsp.uParam.sRspAssociate.u8Status = u8AssocStatus;
	sReqRsp.uParam.sRspAssociate.u8SecurityEnable = bSecurity;
	
	/* Send association response. There is no confirmation for an association
	 * response, hence no need to check */
	vAppApiMlmeRequest(&sReqRsp, &sMlmeSyncCfm);

	IEEE802154_DBG_PRINTLN("sReqRsp.u8Type %04x", sReqRsp.u8Type);
	IEEE802154_DBG_PRINTLN("sReqRsp.u8ParamLength %04x", sReqRsp.u8ParamLength);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sRspAssociate.sDeviceAddr.u32H %04x", sReqRsp.uParam.sRspAssociate.sDeviceAddr.u32H);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sRspAssociate.sDeviceAddr.u32L %04x", sReqRsp.uParam.sRspAssociate.sDeviceAddr.u32L);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sRspAssociate.u16AssocShortAddr %04x", sReqRsp.uParam.sRspAssociate.u16AssocShortAddr);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sRspAssociate.u8Status %04x", sReqRsp.uParam.sRspAssociate.u8Status);
	IEEE802154_DBG_PRINTLN("sReqRsp.uParam.sRspAssociate.u8SecurityEnable %04x", sReqRsp.uParam.sRspAssociate.u8SecurityEnable);
}
