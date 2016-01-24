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

extern "C" {
#include <strings.h>
#include <net/ip/resolv.h>
#include <net/ip/uip-nameserver.h>
#include <net/ipv6/uip-nd6.h>
#include <net/ipv6/uip-ds6.h>
#include <net/rpl/rpl.h>
#define DEBUG DEBUG_NONE
#include <net/ip/uip-debug.h>
}

#include "MicroIp.h"

struct resolv_params {
  const char* host;
  uip_ipaddr_t* ipaddr;
  resolv_status_t retval;
};

struct set_addr_params {
  IPAddress* ipaddr;
  uip_ds6_addr_t* locaddr;
};

struct update_nameserver_params {
  uip_ipaddr_t* ipaddr;
  uint32_t lifetime;
};

int MicroIPClass::begin()
{
	//TODO
	return 1;
}

int MicroIPClass::maintain(){
  //Contiki is NOT support DHCPv6.
  return 0;
}

IPAddress MicroIPClass::localIP()
{
  //TODO
  IPAddress ret;

  return ret;
}

IPAddress MicroIPClass::gatewayIP()
{
  //TODO
  IPAddress ret;
  return ret;
}

IPAddress MicroIPClass::dnsServerIP()
{
  //TODO
  IPAddress ret;
  return ret;
}

void MicroIPClass::do_uip_nameserver_update(void* ptr)
{
  struct update_nameserver_params *params= reinterpret_cast<struct update_nameserver_params*>(ptr);
  PRINT6ADDR(params->ipaddr);
  uip_nameserver_update(params->ipaddr, params->lifetime);
}

void MicroIPClass::addDNS(const IPAddress& dnsaddr, uint32_t lifetime)
{
  uip_ipaddr_t ipaddr;
  uip_ipaddr_IPAddress(&ipaddr, dnsaddr);
  struct update_nameserver_params params = { &ipaddr, lifetime };
  yield_continue(do_uip_nameserver_update, &params);
}

IPAddress MicroIPClass::lookup(const char* host)
{
  uip_ipaddr_t ipaddr;
  struct resolv_params params = { host, &ipaddr, RESOLV_STATUS_ERROR };

  yield_continue(do_resolv_lookup, &params);
  if(params.retval == RESOLV_STATUS_UNCACHED || params.retval == RESOLV_STATUS_EXPIRED) {
    params.retval = RESOLV_STATUS_ERROR;
    yield_until(do_resolv_query, &params, wait_resolv_event_found, &params);
    yield_continue(do_resolv_lookup, &params);
  }
  if(params.retval == RESOLV_STATUS_CACHED && params.ipaddr != NULL) {
    return IPAddress(params.ipaddr->u8);
  }
  return INADDR_NONE;
}

void MicroIPClass::do_resolv_set_hostname(void* ptr)
{
  const char* hostname = reinterpret_cast<const char*>(ptr);
  PRINTF("resolv_set_hostname %s\n", hostname);
  if( !process_is_running(&resolv_process) ) {
    process_start(&resolv_process, NULL);
  }
  resolv_set_hostname(hostname);
}

void MicroIPClass::setHostname(const char* hostname)
{
  yield_continue(do_resolv_set_hostname, const_cast<char*>(hostname) );
}

void MicroIPClass::do_resolv_query(void* ptr)
{
  struct resolv_params* params = reinterpret_cast<struct resolv_params*>(ptr);
  resolv_query(params->host);
}

void MicroIPClass::do_resolv_lookup(void* ptr)
{
  struct resolv_params* params = reinterpret_cast<struct resolv_params*>(ptr);
  resolv_status_t status = RESOLV_STATUS_ERROR;

  uip_ipaddr_t *resolved_addr = NULL;
  status = resolv_lookup(params->host, &resolved_addr);
  if(status == RESOLV_STATUS_UNCACHED || status == RESOLV_STATUS_EXPIRED) {
    PRINTF("Attempting to look up %s\n",params->host);
  } else if(status == RESOLV_STATUS_CACHED && resolved_addr != NULL) {
    PRINTF("Lookup of \"%s\" succeded!\n", params->host);
  } else if(status == RESOLV_STATUS_RESOLVING) {
    PRINTF("Still looking up \"%s\"...\n", params->host);
  } else {
    PRINTF("Lookup of \"%s\" failed. status = %d\n", params->host, status);
  }

  if(resolved_addr) {
    uip_ipaddr_copy(params->ipaddr, resolved_addr);
  }

  params->retval = status;
}

int MicroIPClass::wait_resolv_event_found(process_event_t ev, process_data_t data, void* ptr)
{
  struct resolv_params* params = reinterpret_cast<struct resolv_params*>(ptr);
  return ( (ev == resolv_event_found) && (strcasecmp(params->host, reinterpret_cast<char*>(data)) == 0) );
}

MicroIPClass MicroIP;
