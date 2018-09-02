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
#include <uip.h>
#include <uiplib.h>
#include <net/ip/uip.h>
#include <net/ip/ip64-addr.h>
}
#include <Print.h>
#include <IPAddress.h>

#if NETSTACK_CONF_WITH_IPV6
#define INIT_IPBYTES(a, b, c, d) {0,0,0,0,  0,0,0,0,  0,0,0xFF,0xFF,  a,b,c,d}
#else
#define INIT_IPBYTES(a, b, c, d) {a,b,c,d}
#endif

IPAddress::IPAddress()
{
    uint8_t addrbytes[] = INIT_IPBYTES(0, 0, 0, 0);
    memcpy(_address.u8, addrbytes, sizeof(addrbytes));
}

IPAddress::IPAddress(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
    uint8_t addrbytes[] = INIT_IPBYTES(b0, b1, b2, b3);
    memcpy(_address.u8, addrbytes, sizeof(addrbytes));
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

IPAddress::IPAddress(const uint16_t *addr)
{
    memcpy(_address.u16, addr, sizeof(_address.u16));
}
#endif

IPAddress::IPAddress(uint32_t addr)
{
    uint8_t addrbytes[] = INIT_IPBYTES(
        (uint8_t)((addr & 0xFF000000) >> 24),
        (uint8_t)((addr & 0x00FF0000) >> 16),
        (uint8_t)((addr & 0x0000FF00) >>  8),
        (uint8_t)((addr & 0x000000FF) >>  0) );
    memcpy(_address.u8, addrbytes, sizeof(addrbytes));
}

IPAddress::IPAddress(const uint8_t *addr)
{
    const uint8_t addrbytes[] = INIT_IPBYTES(addr[0], addr[1], addr[2], addr[3]);
    memcpy(_address.u8, addrbytes, sizeof(addrbytes));

}

bool IPAddress::fromString(const char *addr)
{
    int ret = uiplib_ipaddrconv(addr, reinterpret_cast<uip_ipaddr_t*>(&_address));
    return (ret != 0);
}

IPAddress& IPAddress::operator=(const uint8_t *addr)
{
    const uint8_t addrbytes[] = INIT_IPBYTES(addr[0], addr[1], addr[2], addr[3]);
    memcpy(_address.u8, addrbytes, sizeof(addrbytes));
    return *this;
}

IPAddress& IPAddress::operator=(uint32_t addr)
{
    uint8_t addrbytes[] = INIT_IPBYTES(
        (uint8_t)((addr & 0xFF000000) >> 24),
        (uint8_t)((addr & 0x00FF0000) >> 16),
        (uint8_t)((addr & 0x0000FF00) >>  8),
        (uint8_t)((addr & 0x000000FF) >>  0) );
    memcpy(_address.u8, addrbytes, sizeof(addrbytes));
    return *this;
}

bool IPAddress::operator==(const uint8_t* addr) const
{
    return memcmp(addr, _address.u8, sizeof(_address.u8)) == 0;
}

#if NETSTACK_CONF_WITH_IPV6
bool IPAddress::operator==(const uint16_t* addr) const
{
    return memcmp(addr, _address.u16, sizeof(_address.u16)) == 0;
}
#endif

#if NETSTACK_CONF_WITH_IPV6
IPAddress& IPAddress::operator=(const IPAddress& addr)
{
    *this = addr._address.u16;
    return *this;
}

IPAddress& IPAddress::operator=(const uint16_t *addr)
{
    memcpy(_address.u16, addr, sizeof(_address.u16));
    return *this;
}
#endif

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
    if(ip64_addr_is_ipv4_mapped_addr(reinterpret_cast<const uip_ipaddr_t*>(&_address)))
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

