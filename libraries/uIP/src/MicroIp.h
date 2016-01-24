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

#define uip_ipaddr_IPAddress(addr, ipaddr) uip_ip6addr_u8(addr, \
		ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3],\
		ipaddr[4], ipaddr[5], ipaddr[6], ipaddr[7],\
		ipaddr[8], ipaddr[9], ipaddr[10], ipaddr[11],\
		ipaddr[12], ipaddr[13], ipaddr[14], ipaddr[15])

class MicroIPClass {
public:

  int begin();
  int maintain();

  IPAddress localIP();
  IPAddress subnetMask();
  IPAddress gatewayIP();
  IPAddress dnsServerIP();

#if NETSTACK_CONF_WITH_IPV6
  IPAddress interfaceID();
  IPAddress prefix();
  unsigned int prefixLength();
  IPAddress lookup(const char* host);
  void setHostname(const char* hostname);
  void addDNS(const IPAddress& addr, uint32_t lifetime=0xFFFFFFFF);
  void removeDNS(const IPAddress& addr) { addDNS(addr, 0); }
#endif

private:
  static void do_resolv_lookup(void* ptr);
  static void do_resolv_query(void* ptr);
  static void do_resolv_set_hostname(void* ptr);
  static void do_uip_nameserver_update(void* ptr);
  static  int wait_resolv_event_found(process_event_t ev, process_data_t data, void* param);

  friend class MicroIPClient;
  friend class MicroIPServer;
};

extern MicroIPClass MicroIP;

#endif
