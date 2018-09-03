/*
  IPAddress.h - Base class that provides IPAddress
  Copyright (c) 2011 Adrian McEwen.  All right reserved.
  Copyright (c) 2014-2018 Tokita, Hiroshi

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

// A class to make it easier to handle and pass around IP addresses

class IPAddress : public Printable {
private:
    union {
#if NETSTACK_CONF_WITH_IPV6
      uint16_t u16[8];
      uint8_t u8[16];
      struct _v4map_data {
        uint8_t prefix[12];
        union _v4_data{
          uint8_t bytes[4];
          uint32_t dword;
        } v4;
      } v4map;
#else
      uint16_t u16[2];
      uint8_t u8[4];
      struct _v4map_data {
        uint8_t prefix[0];
        union _v4_data{
          uint8_t bytes[4];
          uint32_t dword;
        } v4;
      } v4map;
#endif
    } _address;

    // Access the raw byte array containing the address.  Because this returns a pointer
    // to the internal structure rather than a copy of the address this function should only
    // be used when you know that the usage of the returned uint8_t* will be transient and not
    // stored.
    uint8_t* raw_address() { return _address.v4map.v4.bytes; };

public:
    // Constructors
    IPAddress();
    IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    IPAddress(uint32_t address);
    IPAddress(const uint8_t *address);
#if NETSTACK_CONF_WITH_IPV6
    IPAddress(uint16_t d1, uint16_t d2, uint16_t d3, uint16_t d4,
              uint16_t d5, uint16_t d6, uint16_t d7, uint16_t d8);
    IPAddress(const uint16_t *address);
#endif

    bool fromString(const char *address);
    bool fromString(const String &address) { return fromString(address.c_str()); }

    // Overloaded cast operator to allow IPAddress objects to be used where a pointer
    // to a four-byte uint8_t array is expected
    operator uint32_t() const { return _address.v4map.v4.dword; };
    bool operator==(const IPAddress& addr) const { return ((*this) == addr._address.u16); };
    bool operator==(const uint8_t* addr) const;
#if NETSTACK_CONF_WITH_IPV6
    bool operator==(const uint16_t* addr) const;
#endif

    // Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator[](int index) const { return _address.v4map.v4.bytes[index]; };
    uint8_t& operator[](int index) { return _address.v4map.v4.bytes[index]; };

    // Overloaded copy operators to allow initialisation of IPAddress objects from other types
    IPAddress& operator=(const uint8_t *address);
    IPAddress& operator=(uint32_t address);
#if NETSTACK_CONF_WITH_IPV6
    IPAddress& operator=(const IPAddress& addr);
    IPAddress& operator=(const uint16_t *address);
#endif

    virtual size_t printTo(Print& p) const;

    friend class EthernetClass;
    friend class UDP;
    friend class Client;
    friend class Server;
    friend class DhcpClass;
    friend class DNSClient;

    class V6RawAccessor : public Printable
    {
        uint16_t* addr;
        V6RawAccessor(uint16_t* a) : addr(a) { }
        uint16_t* raw_address() { return addr; }

        public:
        uint16_t operator[](int index) const { return addr[index]; }
        uint16_t& operator[](int index) { return addr[index]; }
        virtual size_t printTo(Print& p) const;

        friend class IPAddress;
    };

#if !NETSTACK_CONF_WITH_IPV6
private:
#endif
    V6RawAccessor v6;
};

const IPAddress INADDR_NONE(0,0,0,0);
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
