/*
  Copyright (c) 2016-2018 Tokita, Hiroshi  All right reserved.

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
#include <net/ip/uiplib.h>
#include <net/ip/uip-nameserver.h>
#include <net/ipv6/uip-nd6.h>
#include <net/ipv6/uip-ds6.h>
#include <net/rpl/rpl.h>
#define DEBUG DEBUG_NONE
#include <net/ip/uip-debug.h>
}

#include "wiring_private.h"
#include "MicroIp.h"

static resolv_status_t set_connection_address(uip_ipaddr_t *ipaddr, const char* host);
static void do_set_hostname_and_query(void* ptr);
static void do_resolv_query(void* ptr);
static int wait_resolv_event_found(process_event_t ev, process_data_t data, void* ptr);
static void do_uip_nameserver_update(void* ptr);


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
  uip_ipaddr_t ipaddr;

  PRINTF("uip_ip6addr(UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);\n");
  uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
  PRINTF("uip_ds6_set_addr_iid(");
  PRINT6ADDR(&ipaddr);
  PRINTF(", ");
  PRINTLLADDR(&uip_lladdr);
  PRINTF(")\n");
  uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
  PRINTF("uip_ds6_addr_add(");
  PRINT6ADDR(&ipaddr);
  PRINTF(")\n");
  uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

  process_start(&resolv_process, NULL);
  return 1;
}

int MicroIPClass::maintain(){
  //Contiki is NOT support DHCPv6.
  return 0;
}

IPAddress MicroIPClass::linklocalAddress()
{
  return linklocalAddress(ADDR_PREFERRED);
}

IPAddress MicroIPClass::linklocalAddress(int state)
{
  uip_ds6_addr_t* addr = uip_ds6_get_link_local(state);
  if(addr != NULL)
  {
    return IPAddress_from_uip(addr->ipaddr);
  }
  return IPAddress();
}

IPAddress MicroIPClass::globalAddress()
{
  return globalAddress(ADDR_PREFERRED);
}

IPAddress MicroIPClass::globalAddress(int state)
{
  uip_ds6_addr_t* addr = uip_ds6_get_global(state);
  if(addr != NULL)
  {
    return IPAddress_from_uip(addr->ipaddr);
  }
  return IPAddress();
}

IPAddress MicroIPClass::interfaceID()
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

static void do_uip_nameserver_update(void* ptr)
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
  static resolv_status_t status = RESOLV_STATUS_UNCACHED;

  while(status != RESOLV_STATUS_CACHED) {
    PRINTF("set_connection_address(%s)\n", host);
    status = set_connection_address(&ipaddr, host);
    PRINTF("set_connection_address status=%d addr=", status);
    PRINT6ADDR(&ipaddr);
    PRINTF("\n");

    if(status == RESOLV_STATUS_UNCACHED || status == RESOLV_STATUS_EXPIRED) {
      struct resolv_params params = { host, &ipaddr, RESOLV_STATUS_ERROR };
      PRINTF("Wait resolv_event_found\n");
      yield_until(do_resolv_query, &params, wait_resolv_event_found, &params);
    } else if(status != RESOLV_STATUS_CACHED) {
      PRINTF("Can't get connection address.\n");
      return IN6ADDR_ANY_INIT;
    }
  }
  if(status == RESOLV_STATUS_CACHED) {
    return IPAddress_from_uip(ipaddr);
  }
  return IN6ADDR_ANY_INIT;
}

static void do_set_hostname_and_query(void* ptr)
{
  struct resolv_params* params = reinterpret_cast<struct resolv_params*>(ptr);
  resolv_set_hostname(params->host);
  PRINTF("resolv_query(%s)\n", params->host);
  resolv_query(params->host);
}

void MicroIPClass::setHostname(const char* hostname)
{
#if RESOLV_CONF_SUPPORTS_MDNS
  static char hostname_local[255];
  uip_ipaddr_t ipaddr;

  strcpy(hostname_local, hostname);
  strcpy(hostname_local + strlen(hostname_local), ".local");

  if(hostname)
  {
    // Issue self-hostname query to ensure which is can annouce hostname.
    struct resolv_params params = { hostname_local, &ipaddr, RESOLV_STATUS_ERROR };
    PRINTF("enter do_set_hostname_and_query\n");
    yield_until(do_set_hostname_and_query, &params, wait_resolv_event_found, &params);
    PRINTF("exit  do_set_hostname_and_query\n");
  }
#endif
}

static void do_resolv_query(void* ptr)
{
  struct resolv_params* params = reinterpret_cast<struct resolv_params*>(ptr);
  PRINTF("resolv_query(%s)\n", params->host);
  resolv_query(params->host);
}

static resolv_status_t set_connection_address(uip_ipaddr_t *ipaddr, const char* host)
{
  resolv_status_t status = RESOLV_STATUS_ERROR;

  if(uiplib_ipaddrconv(host, ipaddr) == 0) {
    uip_ipaddr_t *resolved_addr = NULL;

    //NOTE: resolv_lookup don't require process context.
    PRINTF("uiplib_ipaddrconv == 0\n");
    PRINTF("resolv_lookup(%s)\n", host);
    status = resolv_lookup(host,&resolved_addr);
    PRINTF("resolv_lookup status = %d resolved_addr = ", status);
    PRINT6ADDR(resolved_addr);
    PRINTF("\n");
    if(status == RESOLV_STATUS_UNCACHED || status == RESOLV_STATUS_EXPIRED) {
      PRINTF("Attempting to look up %s\n",host);
    } else if(status == RESOLV_STATUS_CACHED && resolved_addr != NULL) {
      PRINTF("Lookup of \"%s\" succeded!\n",host);
    } else if(status == RESOLV_STATUS_RESOLVING) {
      PRINTF("Still looking up \"%s\"...\n",host);
    } else {
      PRINTF("Lookup of \"%s\" failed. status = %d\n",host, status);
    }
    if(resolved_addr)
      uip_ipaddr_copy(ipaddr, resolved_addr);
  } else {
    PRINTF("uiplib_ipaddrconv != 0\n");
    status = RESOLV_STATUS_CACHED;
  }

  return status;
}

static int wait_resolv_event_found(process_event_t ev, process_data_t data, void* ptr)
{
  struct resolv_params* params = reinterpret_cast<struct resolv_params*>(ptr);
  return ( (ev == resolv_event_found) && (strcasecmp(params->host, reinterpret_cast<char*>(data)) == 0) );
}

MicroIPClass MicroIP;
