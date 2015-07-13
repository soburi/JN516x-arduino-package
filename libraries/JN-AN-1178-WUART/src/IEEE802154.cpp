#include "IEEE802154.hpp"

#include <mac_sap.h>
#include <mac_pib.h>
#include <AppApi.h>

#include <Arduino.h>

#define IEEE802154_DBG_PRINTF(...) DBG_PRINTF(__VA_ARGS__)
#define IEEE802154_DBG_PRINTLN(...) { DBG_PRINTF(__VA_ARGS__); DBG_PRINTF("\r\n"); }
#define IEEE802154_DBG_NL() DBG_PRINTF("\r\n")

//#define IEEE802154_TRACE_FUNC

#ifdef IEEE802154_TRACE_FUNC
#define IEEE802154_DBG_FUNC_ENTER() IEEE802154_DBG_PRINTLN(__PRETTY_FUNCTION__)
#define IEEE802154_DBG_FUNC_PARAMD(x) {(void)x; IEEE802154_DBG_PRINTF(":" #x ":%d ", x) }
#define IEEE802154_DBG_FUNC_PARAMX(x) {(void)x; IEEE802154_DBG_PRINTF(":" #x ":0x%x ", x) }
#define IEEE802154_DBG_FUNC_PARAMLN(...) IEEE802154_DBG_PRINTLN(__VA_ARGS__)
#define IEEE802154_DBG_FUNC_NL() IEEE802154_DBG_NL()
#else
#define IEEE802154_DBG_FUNC_ENTER()
#define IEEE802154_DBG_FUNC_PARAMD(x) {(void)x;}
#define IEEE802154_DBG_FUNC_PARAMX(x) {(void)x;}
#define IEEE802154_DBG_FUNC_PARAMLN(...)
#define IEEE802154_DBG_FUNC_NL()
#endif

#ifdef IEEE802154_TRACE_STATE
#define IEEE802154_DBG_STATE(...) DBG_PRINTLN(__VA_ARGS__)
#else
#define IEEE802154_DBG_STATE(...)
#endif

void IEEE802154::handleMlmeDcfmScan(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_MlmeDcfmInd_s& sMlmeReqRsp = *psMlmeInd;

	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.u8Type %04x", sMlmeReqRsp.u8Type);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.u8ParamLength %04x", sMlmeReqRsp.u8ParamLength);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.u8Status %04x", sMlmeReqRsp.uParam.sDcfmScan.u8Status);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.u8ScanType %04x", sMlmeReqRsp.uParam.sDcfmScan.u8ScanType);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.u8ResultListSize %04x", sMlmeReqRsp.uParam.sDcfmScan.u8ResultListSize);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.u32UnscannedChannels %04x", sMlmeReqRsp.uParam.sDcfmScan.u32UnscannedChannels);

	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].sCoord.u16PanId %04x", sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].sCoord.u16PanId);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u16SuperframeSpec %04x", sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u16SuperframeSpec);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u32TimeStamp %04x", sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u32TimeStamp);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8AclEntry %04x", sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8AclEntry);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8GtsPermit %04x", sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8GtsPermit);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8LinkQuality %04x", sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8LinkQuality);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8LogicalChan %04x", sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8LogicalChan);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8SecurityFailure %04x", sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8SecurityFailure);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8SecurityUse %04x", sMlmeReqRsp.uParam.sDcfmScan.uList.asPanDescr[0].u8SecurityUse);

	MAC_MlmeCfmScan_s* psDcfmScan = &sMlmeReqRsp.uParam.sDcfmScan;
	m_ScanInd = *psMlmeInd;
	if (psDcfmScan->u8Status == MAC_ENUM_SUCCESS)	{
		confirmScanSuccess(psDcfmScan->u8ScanType, psDcfmScan->u8ResultListSize, psDcfmScan->u32UnscannedChannels);
	}
	else {
		confirmScanFailed(psDcfmScan->u8Status);
	}
}

void IEEE802154::handleMlmeDcfmGts(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	IEEE802154_DBG_FUNC_ENTER();
	MAC_MlmeCfmGts_s* psDcfmGts = &psMlmeInd->uParam.sDcfmGts;
	if (psDcfmGts->u8Status == MAC_ENUM_SUCCESS)
	{
		confirmGtsSuccess(psDcfmGts->u8Characteristics);
	}
	else
	{
		confirmGtsFailed(psDcfmGts->u8Status);
	}
}

void IEEE802154::handleMlmeDcfmAssociate(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	IEEE802154_DBG_FUNC_ENTER();

	MAC_MlmeDcfmInd_s& sMlmeReqRsp = *psMlmeInd;

	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.u8Type %04x", sMlmeReqRsp.u8Type);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.u8ParamLength %04x", sMlmeReqRsp.u8ParamLength);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmAssociate.u8Status %04x", sMlmeReqRsp.uParam.sDcfmAssociate.u8Status);
	IEEE802154_DBG_PRINTLN("sMlmeReqRsp.uParam.sDcfmAssociate.u16AssocShortAddr %04x", sMlmeReqRsp.uParam.sDcfmAssociate.u16AssocShortAddr);

	MAC_MlmeCfmAssociate_s* psDcfmAssociate = &sMlmeReqRsp.uParam.sDcfmAssociate;
	if (psDcfmAssociate->u8Status == MAC_ENUM_SUCCESS)
	{
		confirmAssociateSuccess(psDcfmAssociate->u16AssocShortAddr);
	}
	else
	{
		confirmAssociateFailed(psDcfmAssociate->u8Status);
	}
}

void IEEE802154::handleMlmeDcfmDisassociate(MAC_MlmeDcfmInd_s* psMlmeInd) {
	IEEE802154_DBG_FUNC_ENTER();
	MAC_MlmeCfmDisassociate_s* psDcfmDisassociate = &psMlmeInd->uParam.sDcfmDisassociate;
	if (psDcfmDisassociate->u8Status == MAC_ENUM_SUCCESS)
	{
		confirmDisassociateSuccess();
	}
	else
	{
		confirmDisassociateFailed(psDcfmDisassociate->u8Status);
	}
}

void IEEE802154::handleMlmeDcfmPoll(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	IEEE802154_DBG_FUNC_ENTER();
	if (psMlmeInd->uParam.sDcfmPoll.u8Status == MAC_ENUM_SUCCESS)
	{
		confirmPollSuccess();
	}
	else
	{
		confirmPollFailed(psMlmeInd->uParam.sDcfmDisassociate.u8Status);
	}
}

void IEEE802154::handleMlmeDcfmRxEnable(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	IEEE802154_DBG_FUNC_ENTER();
	if (psMlmeInd->uParam.sDcfmRxEnable.u8Status == MAC_ENUM_SUCCESS)
	{
		confirmRxEnableSuccess();
	}
	else
	{
		confirmRxEnableFailed(psMlmeInd->uParam.sDcfmRxEnable.u8Status);
	}
}


void IEEE802154::handleMcpsDcfmData(MAC_McpsDcfmInd_s* psDcfmInd)
{
	IEEE802154_DBG_FUNC_ENTER();
	MAC_McpsCfmData_s* psDcfmData = &(psDcfmInd->uParam.sDcfmData);
	if (psDcfmData->u8Status == MAC_ENUM_SUCCESS)
	{
		confirmDataSuccess(psDcfmData->u8Handle, psDcfmData->u32Timestamp);
	}
	else
	{
		confirmDataFailed(psDcfmData->u8Status);
	}
}

void IEEE802154::handleMcpsDcfmPurge(MAC_McpsDcfmInd_s* psDcfmInd)
{
	IEEE802154_DBG_FUNC_ENTER();
	MAC_McpsCfmPurge_s* psDcfmPurge = &(psDcfmInd->uParam.sDcfmPurge);
	if (psDcfmPurge->u8Status == MAC_ENUM_SUCCESS)
	{
		confirmPurgeSuccess(psDcfmPurge->u8Handle);
	}
	else
	{
		confirmPurgeFailed(psDcfmPurge->u8Status);
	}
}

void IEEE802154::handleMlmeIndAssociate(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	IEEE802154_DBG_FUNC_ENTER();
	MAC_MlmeIndAssociate_s* psIndAssociate = &(psMlmeInd->uParam.sIndAssociate);
	indicateAssociate(psIndAssociate->sDeviceAddr.u32H, psIndAssociate->sDeviceAddr.u32L, psIndAssociate->u8Capability);
}

void IEEE802154::handleMlmeIndDisassociate(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	IEEE802154_DBG_FUNC_ENTER();
	MAC_MlmeIndDisassociate_s* psIndDisassociate = &(psMlmeInd->uParam.sIndDisassociate);
	indicateDisassociate(psIndDisassociate->sDeviceAddr.u32H, psIndDisassociate->sDeviceAddr.u32L, psIndDisassociate->u8Reason);
}

void IEEE802154::handleMlmeIndSyncLoss(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	IEEE802154_DBG_FUNC_ENTER();
	MAC_MlmeIndSyncLoss_s* psIndSyncLoss = &(psMlmeInd->uParam.sIndSyncLoss);
	indicateSyncLoss(psIndSyncLoss->u8Reason);
}

void IEEE802154::handleMlmeIndGts(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	IEEE802154_DBG_FUNC_ENTER();
	MAC_MlmeIndGts_s* psIndGts = &(psMlmeInd->uParam.sIndGts);
	indicateGts(psIndGts->u16ShortAddr, psIndGts->u8Characteristics);
}

void IEEE802154::handleMlmeIndBeaconNotify(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	(void)psMlmeInd;
	IEEE802154_DBG_FUNC_ENTER();
//	MAC_MlmeIndBeacon_s* psIndBeacon = &(psMlmeInd->uParam.sIndBeacon);

#if 0
    MAC_PanDescr_s   sPANdescriptor;                    /**< PAN descriptor */
    uint8            u8BSN;                             /**< Beacon sequence number */
    uint8            u8PendAddrSpec;                    /**< Pending address specification */
    uint8            u8SDUlength;                       /**< Length of following payload */
    MAC_Addr_u       uAddrList[7];                      /**< Pending addresses */
    uint8            u8SDU[MAC_MAX_BEACON_PAYLOAD_LEN]; /**< Beacon payload */
#endif
//	indicateBeacon(0,
//			psIndBeacon->u8BSN, psIndBeacon->u8PendAddrSpec, psIndBeacon->u8SDUlength );
}

void IEEE802154::handleMlmeIndCommStatus(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	IEEE802154_DBG_FUNC_ENTER();
	MAC_MlmeIndCommStatus_s* psIndCommStatus = &(psMlmeInd->uParam.sIndCommStatus);
	indicateCommStatus(psIndCommStatus->sSrcAddr.uAddr.u16Short,
			psIndCommStatus->sDstAddr.uAddr.u16Short,
			psIndCommStatus->u8Status);
}

void IEEE802154::handleMlmeIndOrphan(MAC_MlmeDcfmInd_s* psMlmeInd)
{
	IEEE802154_DBG_FUNC_ENTER();
	MAC_MlmeIndOrphan_s* psIndOrphan = &(psMlmeInd->uParam.sIndOrphan);
	indicateOrphan(psIndOrphan->sDeviceAddr.u32H, psIndOrphan->sDeviceAddr.u32L);
}

void IEEE802154::handleMcpsIndData(MAC_McpsDcfmInd_s* psDcfmInd)
{
	//IEEE802154_DBG_FUNC_ENTER();
	MAC_McpsIndData_s* psIndData = &(psDcfmInd->uParam.sIndData);
	indicateData(psIndData->sFrame.sSrcAddr.uAddr.u16Short, psIndData->sFrame.sDstAddr.uAddr.u16Short, psIndData->sFrame.au8Sdu, psIndData->sFrame.u8SduLength);
}


MAC_DcfmIndHdr_s* IEEE802154::MlmeDcfmIndGetBuf(void *pvParam) {
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154* ctx = reinterpret_cast<IEEE802154*>(pvParam);
	return (MAC_DcfmIndHdr_s*)( &(ctx->m_MlmeBuffer) );
}

MAC_DcfmIndHdr_s* IEEE802154::McpsDcfmIndGetBuf(void *pvParam) {
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154* ctx = reinterpret_cast<IEEE802154*>(pvParam);
	return (MAC_DcfmIndHdr_s*)( &(ctx->m_McpsBuffer) );
}

void IEEE802154::MlmeDcfmIndPost(void *pvParam, MAC_DcfmIndHdr_s* psDcfmIndHdr) {
	//IEEE802154_DBG_FUNC_ENTER();
	IEEE802154* ctx = reinterpret_cast<IEEE802154*>(pvParam);
	//IEEE802154_DBG_FUNC_PARAMD(psDcfmIndHdr->u8Type);
	//IEEE802154_DBG_PRINTLN("");

	ctx->handleMlme(psDcfmIndHdr->u8Type, (MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
}

void IEEE802154::handleMlme(uint8_t type, MAC_MlmeDcfmInd_s* psDcfmIndHdr)
{
	//IEEE802154_DBG_FUNC_ENTER();
	switch(type) {
		case MAC_MLME_DCFM_SCAN:
			handleMlmeDcfmScan( (MAC_MlmeDcfmInd_s*) psDcfmIndHdr);
			break;
		case MAC_MLME_DCFM_GTS:
			handleMlmeDcfmGts((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MLME_DCFM_ASSOCIATE:
			handleMlmeDcfmAssociate((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MLME_DCFM_DISASSOCIATE:
			handleMlmeDcfmDisassociate((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MLME_DCFM_POLL:
			handleMlmeDcfmPoll((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MLME_DCFM_RX_ENABLE:
			handleMlmeDcfmRxEnable((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MLME_IND_ASSOCIATE:
			handleMlmeIndAssociate((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MLME_IND_DISASSOCIATE:
			handleMlmeIndDisassociate((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MLME_IND_SYNC_LOSS:
			handleMlmeIndSyncLoss((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MLME_IND_GTS:
			handleMlmeIndGts((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MLME_IND_BEACON_NOTIFY:
			handleMlmeIndBeaconNotify((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MLME_IND_COMM_STATUS:
			handleMlmeIndCommStatus((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MLME_IND_ORPHAN:
			handleMlmeIndOrphan((MAC_MlmeDcfmInd_s*)psDcfmIndHdr);
			break;
#ifdef TOF_ENABLED
		case MAC_MLME_DCFM_TOFPOLL:
		case MAC_MLME_DCFM_TOFPRIME:
		case MAC_MLME_DCFM_TOFDATAPOLL:
		case MAC_MLME_DCFM_TOFDATA:
		case MAC_MLME_IND_TOFPOLL:
		case MAC_MLME_IND_TOFPRIME:
		case MAC_MLME_IND_TOFDATAPOLL:
		case MAC_MLME_IND_TOFDATA:
#endif
#if defined(DEBUG) && defined(EMBEDDED)
		case MAC_MLME_IND_VS_WPAN_CB_DEBUG_INFO = 0xF0:
		case MAC_MLME_IND_VS_WPAN_CB_DEBUG_WARN:
		case MAC_MLME_IND_VS_WPAN_CB_DEBUG_ERROR:
		case MAC_MLME_IND_VS_WPAN_CB_DEBUG_FATAL:
#endif /* defined(DEBUG) && defined(EMBEDDED) */
		default:
		break;
	}
}

void IEEE802154::McpsDcfmIndPost(void *pvParam, MAC_DcfmIndHdr_s* psDcfmIndHdr) {
	//IEEE802154_DBG_FUNC_ENTER();
	IEEE802154* ctx = reinterpret_cast<IEEE802154*>(pvParam);
	//IEEE802154_DBG_FUNC_PARAMD(psDcfmIndHdr->u8Type);
	//IEEE802154_DBG_FUNC_NL();
	ctx->handleMcps(psDcfmIndHdr->u8Type,  (MAC_McpsDcfmInd_s*)psDcfmIndHdr);

}

void IEEE802154::handleMcps(uint8_t type, MAC_McpsDcfmInd_s* psDcfmIndHdr)
{
	//IEEE802154_DBG_FUNC_ENTER();
	switch(type) {
		case MAC_MCPS_DCFM_DATA:
			handleMcpsDcfmData((MAC_McpsDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MCPS_DCFM_PURGE:
			handleMcpsDcfmPurge((MAC_McpsDcfmInd_s*)psDcfmIndHdr);
			break;
		case MAC_MCPS_IND_DATA:
			handleMcpsIndData((MAC_McpsDcfmInd_s*)psDcfmIndHdr);
			break;

#ifdef JENNIC_CHIP_FAMILY_JN516x
		case MAC_MCPS_REQ_DATA_EXTENDED:
#endif
		default:
			break;
	}
}


void IEEE802154::confirmScanSuccess(uint8_t scantype, int scancount, uint32_t channelmask) {
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMD(scantype);
	IEEE802154_DBG_FUNC_PARAMD(scancount);
	IEEE802154_DBG_FUNC_PARAMD(channelmask);
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::confirmFailed(int type, int error)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMD(type);
	IEEE802154_DBG_FUNC_PARAMD(error);
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::confirmAssociateSuccess(uint16_t shortaddr)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMX(shortaddr);
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::confirmDisassociateSuccess()
{
	IEEE802154_DBG_FUNC_ENTER();
}

void IEEE802154::confirmPollSuccess()
{
	IEEE802154_DBG_FUNC_ENTER();
}

void IEEE802154::confirmRxEnableSuccess()
{
	IEEE802154_DBG_FUNC_ENTER();
}
void IEEE802154::confirmGtsSuccess(uint32_t characteristic)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMX(characteristic);
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::confirmDataSuccess(uint8_t u8Handle, uint32_t u32Timestamp)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMD(u8Handle);
	IEEE802154_DBG_FUNC_PARAMD(u32Timestamp);
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::confirmPurgeSuccess(uint8_t u8Handle)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMD(u8Handle);
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::confirmScanFailed(int error)
{
	IEEE802154_DBG_FUNC_ENTER();
	confirmFailed(MAC_MLME_DCFM_SCAN, error);
}

void IEEE802154::confirmAssociateFailed(int error)
{
	IEEE802154_DBG_FUNC_ENTER();
	confirmFailed(MAC_MLME_DCFM_ASSOCIATE, error);
}

void IEEE802154::confirmDisassociateFailed(int error)
{
	IEEE802154_DBG_FUNC_ENTER();
	confirmFailed(MAC_MLME_DCFM_DISASSOCIATE, error);
}

void IEEE802154::confirmPollFailed(int error)
{
	IEEE802154_DBG_FUNC_ENTER();
	confirmFailed(MAC_MLME_DCFM_POLL, error);
}

void IEEE802154::confirmRxEnableFailed(int error)
{
	IEEE802154_DBG_FUNC_ENTER();
	confirmFailed(MAC_MLME_DCFM_RX_ENABLE, error);
}

void IEEE802154::confirmGtsFailed(int error)
{
	IEEE802154_DBG_FUNC_ENTER();
	confirmFailed(MAC_MLME_DCFM_GTS, error);
}

void IEEE802154::confirmDataFailed(int error)
{
	IEEE802154_DBG_FUNC_ENTER();
	confirmFailed(MAC_MCPS_REQ_DATA, error);
}

void IEEE802154::confirmPurgeFailed(int error)
{
	IEEE802154_DBG_FUNC_ENTER();
	confirmFailed(MAC_MCPS_REQ_PURGE, error);
}

void IEEE802154::indicateAssociate(uint32_t addrH, uint32_t addrL, uint8_t capability)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMD(addrH);
	IEEE802154_DBG_FUNC_PARAMD(addrL);
	IEEE802154_DBG_FUNC_PARAMD(capability);
	IEEE802154_DBG_FUNC_NL();
}

#if 0
void IEEE802154::indicateDisassociate(uint32_t addrH, uint32_t addrL, uint8_t  u8Reason)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMX(addrH);
	IEEE802154_DBG_FUNC_PARAMX(addrL);
	IEEE802154_DBG_FUNC_PARAMD(u8Reason);
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::indicateSyncLoss(uint8_t u8Reason)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMD(u8Reason);
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::indicateGts(uint16_t u16ShortAddr, uint8_t u8Characteristics)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMX(u16ShortAddr);
	IEEE802154_DBG_FUNC_PARAMD(u8Characteristics);
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::indicateBeacon()
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::indicateCommStatus(uint16_t u16SrcAddr, uint16_t u16DstAddr, uint8_t u8Status)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMX(u16SrcAddr);
	IEEE802154_DBG_FUNC_PARAMX(u16DstAddr);
	IEEE802154_DBG_FUNC_PARAMD(u8Status);
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::indicateOrphan(uint32_t addrH, uint32_t addrL)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMX(addrH);
	IEEE802154_DBG_FUNC_PARAMX(addrL);
	IEEE802154_DBG_FUNC_NL();
}

void IEEE802154::indicateData(uint16_t u16SrcAddr, uint16_t u16DstAddr, uint8_t* au8Sdu, uint8_t u8SduLength)
{
	IEEE802154_DBG_FUNC_ENTER();
	IEEE802154_DBG_FUNC_PARAMX(u16SrcAddr);
	IEEE802154_DBG_FUNC_PARAMX(u16DstAddr);
	IEEE802154_DBG_FUNC_PARAMD(u8SduLength);
	IEEE802154_DBG_FUNC_NL();
}
#endif

uint32_t IEEE802154::init(uint16_t panid)
{
	IEEE802154_DBG_FUNC_ENTER();

	void *pvMac;
	uint32_t version;

	version = u32AppApiInit(MlmeDcfmIndGetBuf, MlmeDcfmIndPost, this,
			        McpsDcfmIndGetBuf, McpsDcfmIndPost, this);
	IEEE802154_DBG_PRINTLN("pvAppApiGetMacHandle");
	pvMac = pvAppApiGetMacHandle();

	IEEE802154_DBG_PRINTLN("MAC_psPibGetHandle");
	(void)MAC_psPibGetHandle(pvMac);

	IEEE802154_DBG_PRINTF("MAC_vPibSetPanId: 0x%x", panid);
	IEEE802154_DBG_PRINTLN("");
	
	MAC_vPibSetPanId(pvMac, panid);
	
	/* Enable receiver to be on when idle */
	IEEE802154_DBG_PRINTLN("MAC_vPibSetRxOnWhenIdle");
	MAC_vPibSetRxOnWhenIdle(pvMac, 1, FALSE);

	return version;
}


