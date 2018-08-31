/*
  IPAddress.h - Base class that provides IPAddress
  Copyright (c) 2011 Adrian McEwen.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef IPAddress_h
#define IPAddress_h

#include <stdint.h>
#include "Printable.h"
#include "WString.h"

extern "C" {
#include "uip.h"
#include "uiplib.h"
}
// A class to make it easier to handle and pass around IP addresses

class IPAddress : public Printable {
private:
    uip_ipaddr_t _address;

    // Access the raw byte array containing the address.  Because this returns a pointer
    // to the internal structure rather than a copy of the address this function should only
    // be used when you know that the usage of the returned uint8_t* will be transient and not
    // stored.
    uint8_t* raw_address() { return _address.u8; };
    const uip_ipaddr_t* uip_address() const { return &_address; };

public:
    // Constructors
    IPAddress();
    IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
#if NETSTACK_CONF_WITH_IPV6
    IPAddress(uint16_t d1, uint16_t d2, uint16_t d3, uint16_t d4,
		uint16_t d5, uint16_t d6, uint16_t d7, uint16_t d8);
#endif
#if !NETSTACK_CONF_WITH_IPV6
    IPAddress(uint32_t address);
#endif

    IPAddress(const uint8_t *address);

    bool fromString(const char *address);
    bool fromString(const String &address) { return fromString(address.c_str()); }

    // Overloaded cast operator to allow IPAddress objects to be used where a pointer
    // to a four-byte uint8_t array is expected
#if !NETSTACK_CONF_WITH_IPV6
    operator uint32_t() const { 
       return (   _address.u8[0] << 24 | _address.u8[1] << 16
		| _address.u8[2] <<  8 | _address.u8[3]);
    }
#endif
    bool operator==(const IPAddress& addr) const { return uip_ipaddr_cmp(&_address, addr.uip_address() ); };
    bool operator==(const uint8_t* addr) const;

    // Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator[](int index) const { return _address.u8[index]; };
    uint8_t& operator[](int index) { return _address.u8[index]; };

    // Overloaded copy operators to allow initialisation of IPAddress objects from other types
    IPAddress& operator=(const uint8_t *address);
    IPAddress& operator=(const IPAddress& addr);
#if !NETSTACK_CONF_WITH_IPV6
    IPAddress& operator=(uint32_t address);
#endif

    virtual size_t printTo(Print& p) const;

    friend class EthernetClass;
    friend class UDP;
    friend class Client;
    friend class Server;
    friend class DhcpClass;
    friend class DNSClient;
};

const IPAddress INADDR_NONE;
#if NETSTACK_CONF_WITH_IPV6
const IPAddress IN6ADDR_ANY_INIT(0, 0, 0, 0, 0, 0, 0, 0);
const IPAddress IN6ADDR_LOOPBACK_INIT(0, 0, 0, 0, 0, 0, 0, 1);
const IPAddress IN6ADDR_LINKLOCAL_ALLNODES_INIT(  0xff02,0,0,0, 0,0,0,1);
const IPAddress IN6ADDR_LINKLOCAL_ALLROUTERS_INIT(0xff02,0,0,0, 0,0,0,2);
const IPAddress IN6ADDR_INTERFACELOCAL_ALLNODES_INIT(0xff01,0,0,0, 0,0,0,1);
const IPAddress IN6ADDR_INTERFACELOCAL_ALLROUTERS_INIT(0xff01,0,0,0, 0,0,0,2);
const IPAddress IN6ADDR_SITELOCAL_ALLROUTERS_INIT(0xff05,0,0,0, 0,0,0,2);
#endif

#endif
