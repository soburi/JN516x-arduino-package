/*
  Copyright (c) 2016 TOKITA Hiroshi.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef MICROIP_H
#define MICROIP_H

#include <inttypes.h>
#include "IPAddress.h"

#include "MicroIpClient.h"
#include "MicroIpServer.h"
#include "MicroIpRPL.h"

#if NETSTACK_CONF_WITH_IPV6
#define uip_ipaddr_IPAddress(addr, ipaddr) \
    uip_ip6addr((addr), (ipaddr).v6[0], (ipaddr).v6[1], (ipaddr).v6[2], (ipaddr).v6[3],\
                      (ipaddr).v6[4], (ipaddr).v6[5], (ipaddr).v6[6], (ipaddr).v6[7])

#define IPAddress_from_uip(addr) \
    IPAddress((addr).u16[0], (addr).u16[1], (addr).u16[2], (addr).u16[3], \
              (addr).u16[4], (addr).u16[5], (addr).u16[6], (addr).u16[7])
#else
#define uip_ipaddr_IPAddress(addr, ipaddr) \
    uip_(ipaddr)((addr), (ipaddr)[0], (ipaddr)[1], (ipaddr)[2], (ipaddr)[3])
#define IPAddress_from_uip(addr) \
    IPAddress((addr).u8[0], (addr).u8[1], (addr).u8[2], (addr).u8[3])
#endif

class MicroIPClass {
public:

  int begin();
  int maintain();

  IPAddress localIP() { return globalAddress(); }
  IPAddress subnetMask();
  IPAddress gatewayIP();
  IPAddress dnsServerIP();

#if NETSTACK_CONF_WITH_IPV6
  IPAddress linklocalAddress(int state);
  IPAddress linklocalAddress();
  IPAddress globalAddress(int state);
  IPAddress globalAddress();

  IPAddress interfaceID();
  IPAddress prefix();
  unsigned int prefixLength();
  IPAddress lookup(const char* host);
  void setHostname(const char* hostname);
  void addDNS(const IPAddress& addr, uint32_t lifetime=0xFFFFFFFF);
  void removeDNS(const IPAddress& addr) { addDNS(addr, 0); }
#endif

private:
  friend class MicroIPClient;
  friend class MicroIPServer;
};

extern MicroIPClass MicroIP;

#endif
