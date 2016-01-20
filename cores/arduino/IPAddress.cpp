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
#include <net/ip/uip.h>
#include <net/ip/ip64-addr.h>
}
#include <Print.h>
#include <IPAddress.h>

IPAddress::IPAddress()
{
    const uint8_t zero[16] = {0};
    memcpy(_address.u8, zero, sizeof(_address.u8));
}

IPAddress::IPAddress(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
#if !NETSTACK_CONF_WITH_IPV6
    uint8_t addrbytes[4] = { b0, b1, b2, b3};
#else
    // form V6-mapped address.
    uint8_t addrbytes[16] = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0xFF, 0xFF,
            b0, b1, b2, b3};
#endif
    memcpy(_address.u8, addrbytes, sizeof(_address.u8));
}

#if NETSTACK_CONF_WITH_IPV6
IPAddress::IPAddress(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3,
                        uint16_t d4, uint16_t d5, uint16_t d6, uint16_t d7)
{
    _address.u16[0] = d0; _address.u16[1] = d1;
    _address.u16[2] = d2; _address.u16[3] = d3;
    _address.u16[4] = d4; _address.u16[5] = d5;
    _address.u16[6] = d6; _address.u16[7] = d7;
}
#endif

#if !NETSTACK_CONF_WITH_IPV6
IPAddress::IPAddress(uint32_t addr)
{
    _address.u8[0] = (addr & 0xF000) >> 24;
    _address.u8[1] = (addr & 0x0F00) >> 16;
    _address.u8[2] = (addr & 0x00F0) >>  8;
    _address.u8[3] = (addr & 0x000F) >>  0;
}
#endif

IPAddress::IPAddress(const uint8_t *addr)
{
    memcpy(_address.u8, addr, sizeof(_address.u8));
}

bool IPAddress::fromString(const char *addr)
{
    int ret = uiplib_ipaddrconv(addr, &_address);
    return (ret != 0);
}

IPAddress& IPAddress::operator=(const uint8_t *addr)
{
    memcpy(_address.u8, addr, sizeof(_address.u8));
    return *this;
}

IPAddress& IPAddress::operator=(const IPAddress& addr)
{
    uip_ipaddr_copy(&_address, addr.uip_address());
    return *this;
}

#if !NETSTACK_CONF_WITH_IPV6
IPAddress& IPAddress::operator=(uint32_t addr)
{
    _address.u8[0] = (addr & 0xF000) >> 24;
    _address.u8[1] = (addr & 0x0F00) >> 16;
    _address.u8[2] = (addr & 0x00F0) >>  8;
    _address.u8[3] = (addr & 0x000F) >>  0;
    return *this;
}
#endif

bool IPAddress::operator==(const uint8_t* addr) const
{
    return memcmp(addr, _address.u8, sizeof(_address.u8)) == 0;
}

size_t IPAddress::printTo(Print& p) const
{
    size_t n = 0;
#if !NETSTACK_CONF_WITH_IPV6
    for (int i =0; i < 3; i++)
    {
        n += p.print(_address.u8[i], DEC);
        n += p.print('.');
    }
    n += p.print(_address.u8[3], DEC);
#else
    if(ip64_addr_is_ipv4_mapped_addr(&_address))
    {
        n += p.print("::FFFF:");
        n += p.print(_address.u8[12]);
        n += p.print(".");
        n += p.print(_address.u8[13]);
        n += p.print(".");
        n += p.print(_address.u8[14]);
        n += p.print(".");
        n += p.print(_address.u8[15]);
    }
    else
    {
        unsigned int i;
	int f;
        for(i = 0, f = 0; i < sizeof(uip_ipaddr_t); i += 2) {
            uint16_t a = (_address.u8[i] << 8) + _address.u8[i + 1];
            if(a == 0 && f >= 0) {
                if(f++ == 0) {
                    n += p.print("::");
                }
            } else {
                if(f > 0) {
                    f = -1;
                } else if(i > 0) {
                    n += p.print(':');
                }
                n += p.print(a, HEX);
            }
        }
    }
#endif
    return n;
}

