/*
 * IEEE802154.h
 *
 *  Created on: 2015/03/23
 *      Author: crs
 */

#ifndef IEEE802154_H_
#define IEEE802154_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern void IEEE802154_MLME_associate(uint16_t panId, uint8_t logicalChannel, uint16_t coordaddr);
extern void IEEE802154_MLME_disassociate(uint16_t u16Address, uint8_t u8Reason);
extern void IEEE802154_MLME_get(uint8_t pibAttribute, uint8_t pibAttributeIndex);
extern void IEEE802154_MLME_gts(uint8_t characteristics, uint8_t securityEnable);
extern void IEEE802154_MLME_reset(uint8_t u8SetDefaultPib);
extern void IEEE802154_MLME_rx_enable(uint32_t u32RxOnTime, uint32_t u32RxOnDuration, uint8_t  u8DeferPermit);
extern void IEEE802154_MLME_scan(uint8_t scantype, uint32_t u32ChannelstoScan, uint8_t u8ScanDuration);
extern void IEEE802154_MLME_set(uint8_t pibAttribute, uint64_t value, uint8_t pibAttributeIndex);
extern void IEEE802154_MLME_set_buf(uint8_t pibAttribute, uint8_t* au8Buf, uint32_t buflen, uint8_t pibAttributeIndex);
extern void IEEE802154_MLME_start(uint16_t u16PanId, uint8_t  u8Channel, uint8_t u8BeaconOrder,
		uint8_t  u8SuperframeOrder, uint8_t  u8PanCoordinator, uint8_t u8BatteryLifeExt,
		uint8_t  u8Realignment, uint32_t u32StarTime);
extern void IEEE802154_MLME_sync(uint8_t  u8Channel, uint8_t  u8TrackBeacon);
extern void IEEE802154_MLME_poll(uint16_t u16Addr);
extern void IEEE802154_MCPS_data(uint8_t u8Handle, uint16_t u16Panid, uint16_t u16AddressLocal, uint16_t u16AddressRemote, uint8_t *pu8Data, uint8_t u8Length, uint8_t u8TxOptions);
extern void IEEE802154_MCPS_data_ex(uint8_t u8Handle, uint8_t u8Channel, uint16_t u16Panid, uint16_t u16AddressLocal, uint16_t u16AddressRemote, uint8_t *pu8Data, uint8_t u8Length, uint8_t u8TxOptions);
extern void IEEE802154_MCPS_purge(uint8_t u8Handle);
extern void IEEE802154_MLME_resp_associate(uint32_t addrH, uint32_t addrL, uint16_t u16ShortAddress, uint8_t u8AssocStatus, bool bSecurity);

#ifdef __cplusplus
}
#endif


#endif /* IEEE802154_H_ */
