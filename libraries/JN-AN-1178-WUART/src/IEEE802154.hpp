
#ifndef IEEE802154_HPP_
#define IEEE802154_HPP_

#include <mac_sap.h>
#include <mac_pib.h>
#include <stdint.h>
#include "IEEE802154_func.inc"


class IEEE802154 {
	MAC_MlmeDcfmInd_s m_MlmeBuffer;
	MAC_MlmeDcfmInd_s m_McpsBuffer;

	MAC_MlmeDcfmInd_s m_ScanInd;

public:
	virtual ~IEEE802154() {}
	uint32_t init(uint16_t panid);

	inline void requestAssociate(uint16_t panId, uint8_t logicalChannel, uint16_t coordaddr) {
		IEEE802154_MLME_associate(panId, logicalChannel, coordaddr);
	}
	inline void requestDisassociate(uint16_t u16Address, uint8_t u8Reason) {
		IEEE802154_MLME_disassociate(u16Address, u8Reason);
	}
	inline void requestGet(uint8_t pibAttribute, uint8_t pibAttributeIndex) {
		IEEE802154_MLME_get(pibAttribute, pibAttributeIndex);
	}
	inline void requestGts(uint8_t characteristics, uint8_t securityEnable) {
		IEEE802154_MLME_gts(characteristics, securityEnable);
	}
	inline void requestReset(uint8_t u8SetDefaultPib) {
		IEEE802154_MLME_reset(u8SetDefaultPib);
	}
	inline void requestRxEnable(uint32_t u32RxOnTime, uint32_t u32RxOnDuration, uint8_t  u8DeferPermit) {
		IEEE802154_MLME_rx_enable(u32RxOnTime, u32RxOnDuration, u8DeferPermit);
	}
	inline void requestScan(uint8_t scantype, uint32_t u32ChannelstoScan, uint8_t u8ScanDuration) {
		IEEE802154_MLME_scan(scantype, u32ChannelstoScan, u8ScanDuration);
	}
	inline void requestSet(uint8_t pibAttribute, uint64_t value, uint8_t pibAttributeIndex) {
		IEEE802154_MLME_set(pibAttribute, value, pibAttributeIndex);
	}
	inline void requestSetBuf(uint8_t pibAttribute, uint8_t* au8Buf, uint32_t buflen, uint8_t pibAttributeIndex) {
		IEEE802154_MLME_set_buf(pibAttribute, au8Buf, buflen, pibAttributeIndex);
	}
	inline void requestStart(uint16_t u16PanId, uint8_t  u8Channel, uint8_t u8BeaconOrder,
		uint8_t  u8SuperframeOrder, uint8_t  u8PanCoordinator, uint8_t u8BatteryLifeExt,
		uint8_t  u8Realignment, uint32_t u32StarTime) {
		IEEE802154_MLME_start(u16PanId, u8Channel, u8BeaconOrder,
				u8SuperframeOrder, u8PanCoordinator, u8BatteryLifeExt,
				u8Realignment, u32StarTime);
	}
	inline void requestSync(uint8_t  u8Channel, uint8_t  u8TrackBeacon) {
		IEEE802154_MLME_sync(u8Channel, u8TrackBeacon);
	}
	inline void requestPoll(uint16_t u16Addr) {
		IEEE802154_MLME_poll(u16Addr);
	}
	inline void requestData(uint8_t u8Handle, uint16_t u16Panid, uint16_t u16AddressLocal, uint16_t u16AddressRemote, uint8_t *pu8Data, uint8_t u8Length, uint8_t u8TxOptions) {
		IEEE802154_MCPS_data(u8Handle, u16Panid, u16AddressLocal, u16AddressRemote, pu8Data, u8Length, u8TxOptions);
	}
	inline void requestDataEx(uint8_t u8Handle, uint8_t u8Channel, uint16_t u16Panid, uint16_t u16AddressLocal, uint16_t u16AddressRemote, uint8_t *pu8Data, uint8_t u8Length, uint8_t u8TxOptions) {
		IEEE802154_MCPS_data_ex(u8Handle, u8Channel, u16Panid, u16AddressLocal, u16AddressRemote, pu8Data, u8Length, u8TxOptions);
	}
	inline void requestPurge(uint8_t u8Handle) {
		IEEE802154_MCPS_purge(u8Handle);
	}

	inline void reqponseAssociate(uint32_t addrH, uint32_t addrL, uint16_t u16ShortAddress, uint8_t u8AssocStatus, bool bSecurity) {
		IEEE802154_MLME_resp_associate(addrH, addrL, u16ShortAddress, u8AssocStatus, bSecurity);
	}

protected:
	virtual void confirmScanSuccess(uint8_t u8ScanType, int foundcount, uint32_t channelmask);
	virtual void confirmAssociateSuccess(uint16_t shortaddr);
	virtual void confirmDisassociateSuccess();
	virtual void confirmPollSuccess();
	virtual void confirmRxEnableSuccess();
	virtual void confirmGtsSuccess(uint32_t characteristic);
	virtual void confirmDataSuccess(uint8_t u8Handle, uint32_t u32Timestamp);
	virtual void confirmPurgeSuccess(uint8_t u8Handle);

	virtual void confirmScanFailed(int error);
	virtual void confirmAssociateFailed(int error);
	virtual void confirmDisassociateFailed(int error);
	virtual void confirmPollFailed(int error);
	virtual void confirmRxEnableFailed(int error);
	virtual void confirmGtsFailed(int error);
	virtual void confirmDataFailed(int error);
	virtual void confirmPurgeFailed(int error);

	virtual void confirmFailed(int reqtype, int error);

	virtual void indicateAssociate(uint32_t addrH, uint32_t addrL, uint8_t capability);
	virtual void indicateDisassociate(uint32_t addrH, uint32_t addrL, uint8_t  u8Reason);
	virtual void indicateSyncLoss(uint8_t u8Reason);
	virtual void indicateGts(uint16_t u16ShortAddr, uint8_t u8Characteristics);
	virtual void indicateBeacon();
	virtual void indicateCommStatus(uint16_t u16SrcAddr, uint16_t u16DstAddr, uint8_t u8Status);
	virtual void indicateOrphan(uint32_t addrH, uint32_t addrL);
	virtual void indicateData(uint16_t u16SrcAddr, uint16_t u16DstAddr, uint8_t* au8Sdu, uint8_t u8SduLength);

	void handleMlme(uint8_t type, MAC_MlmeDcfmInd_s* buffer);
	void handleMcps(uint8_t type, MAC_McpsDcfmInd_s* buffer);

	//inline MAC_MlmeDcfmInd_s& MlmeBuffer() { return m_ScanInd; }
	inline MAC_MlmeDcfmInd_s& MlmeBuffer() { return m_MlmeBuffer; }
	inline MAC_MlmeDcfmInd_s& McpsBuffer() { return m_McpsBuffer; }

private:
	void handleMlmeDcfmScan(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMlmeDcfmGts(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMlmeDcfmAssociate(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMlmeDcfmDisassociate(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMlmeDcfmPoll(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMlmeDcfmRxEnable(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMcpsDcfmData(MAC_McpsDcfmInd_s* psDcfmInd);
	void handleMcpsDcfmPurge(MAC_McpsDcfmInd_s* psDcfmInd);
	void handleMlmeIndAssociate(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMlmeIndDisassociate(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMlmeIndSyncLoss(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMlmeIndGts(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMlmeIndBeaconNotify(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMlmeIndCommStatus(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMlmeIndOrphan(MAC_MlmeDcfmInd_s* psMlmeInd);
	void handleMcpsIndData(MAC_McpsDcfmInd_s* psDcfmInd);

	static MAC_DcfmIndHdr_s* MlmeDcfmIndGetBuf(void *pvParam);
	static MAC_DcfmIndHdr_s* McpsDcfmIndGetBuf(void *pvParam);
	static void MlmeCallback();
	static void McpsCallback();

	static void MlmeDcfmIndPost(void *pvParam, MAC_DcfmIndHdr_s* psDcfmIndHdr);
	static void McpsDcfmIndPost(void *pvParam, MAC_DcfmIndHdr_s* psDcfmIndHdr);
};

#endif
