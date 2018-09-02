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
#include <net/ip/uip.h>
#include <net/ipv6/uip-ds6.h>
#include <net/rpl/rpl.h>
#define DEBUG DEBUG_NONE
#include <net/ip/uip-debug.h>
}

#include "MicroIpRPL.h"

int MicroIPRPL::begin(const IPAddress& prefix, uint32_t prefix_length)
{
#if UIP_CONF_IPV6_RPL
  rpl_dag_t *dag;
  uip_ipaddr_t ipaddr;
  uip_ipaddr_t prefix_;

  PRINTF("MicroIPClass::begin(prefix)\n");

  uip_ip6addr_u8(&ipaddr,
		  prefix[0],  prefix[1], prefix[2], prefix[3],
		  prefix[4],  prefix[5], prefix[6], prefix[7],
		  prefix[8],  prefix[9],prefix[10],prefix[11],
		  prefix[12],prefix[13],prefix[14],prefix[15]);

  dag = rpl_set_root(RPL_DEFAULT_INSTANCE, &ipaddr);
  if(dag != NULL) {
    rpl_set_prefix(dag, &prefix_, prefix_length);
    PRINTF("created a new RPL dag\n");
    return 1;
  }
#else
  (void)prefix;
  (void)prefix_length;
#endif
  return 0;
}

MicroIPRPL RPL;
