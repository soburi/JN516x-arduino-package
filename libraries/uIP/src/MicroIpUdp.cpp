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
#include "string.h"
#include <net/ip/uip.h>
#include <net/ip/resolv.h>
#define DEBUG DEBUG_NONE
#include <net/ip/uip-debug.h>
}

#include "Arduino.h"
#include "Udp.h"
#include "MicroIP.h"
#include "MicroIPUDP.h"

struct udp_socket_params {
  const MicroIPUDP* udp;
  const uip_ipaddr_t* ipaddr;
  uint16_t port;
  int retval;
};

struct receive_pack {
  uint16_t length;
  uip_ipaddr_t ipaddr;
  uint16_t port;
};

static const int sz_pack = sizeof(struct receive_pack);

MicroIPUDP::MicroIPUDP() : remaining(0)
{
  ringbuf_init(&rxbuf, rbuf, static_cast<uint8_t>(MICROIPUDP_RXBUF_SIZE));
}

uint8_t MicroIPUDP::begin(uint16_t port)
{
  PRINTF("MicroIPUDP::begin(port)\n");
  struct udp_socket_params params = { this, NULL, port, 0 };

  yield_continue(do_udp_socket_register, &params);

  PRINTF("MicroIPUDP::begin(port) reg_params.retval=%d\n", params.retval);
  if(params.retval != 1)
  {
    return 0;
  }

  params.retval = 0;
  yield_continue(do_udp_socket_bind, &params);

  PRINTF("return MicroIPUDP::begin(port) params.retval=%d\n", params.retval);
  if(params.retval != 1)
  {
    return 0;
  }

  return 1;
}

int MicroIPUDP::available()
{
  return remaining;
}

void MicroIPUDP::stop()
{
  udp_socket_close(&sock);
}

int MicroIPUDP::beginPacket(const char *host, uint16_t port)
{
  IPAddress remote = MicroIP.lookup(host);
  if(remote == INADDR_NONE) {
    return 0;
  }
  return beginPacket(remote, port);
}

int MicroIPUDP::beginPacket(IPAddress ip, uint16_t port)
{
  txidx = 0;

  uip_ipaddr_copy(&destIP, uipIPAddress(ip) );
  destPort = port;

  return 1;
}

int MicroIPUDP::endPacket()
{
  struct udp_socket_params params = { this, &destIP, destPort, 0 };
  yield_continue(do_udp_socket_sendto, &params);
  txidx = 0;
  if(params.retval < 0) {
    return 0;
  }
  return 1;
}

size_t MicroIPUDP::write(uint8_t byte)
{
  if( (txidx+1) < UIP_BUFSIZE ) {
    tbuf[txidx++] = byte;
    return 1;
  }
  else {
    return -1;
  }
}

size_t MicroIPUDP::write(const uint8_t *buffer, size_t size)
{
  for(uint32_t i=0; i<size; i++) {
    int r = write(buffer[i]);
    if(r == -1) {
      return i;
    }
  }
  return size;
}

int MicroIPUDP::parsePacket()
{
  PRINTF("MicroIPUDP::parsePacket %d %d\n", ringbuf_elements(&rxbuf), available());
  if( available() ) {
    //notify
    //discard current data
    while( available() ) {
      (void) read();
    }
  }

  struct receive_pack pack;

  // garbage cleanup.
  if( ringbuf_elements(&rxbuf) < sz_pack ) {
    while( raw_read() != -1);
    remaining = 0;
  }
  else {
    raw_read(reinterpret_cast<uint8_t*>(&pack) ,sz_pack);

    remaining = pack.length - sz_pack;
    _remoteIP = pack.ipaddr.u8;
    _remotePort = pack.port;

    PRINTF("return MicroIPUDP::parsePacket %d\n", available());
  }
  return available();
}

int MicroIPUDP::read()
{
  if(remaining) {
    remaining--;
    return raw_read();
  }
  else {
    return -1;
  }
}

int MicroIPUDP::read(unsigned char* buf, size_t size)
{
  for(uint32_t i=0; i<size; i++) {
    int r = read();
    if(r == -1) {
      return i;
    }
    buf[i] = (uint8_t)r;
  }
  return size;
}

int MicroIPUDP::peek()
{
  struct ringbuf *r = &rxbuf;
  if(((r->put_ptr - r->get_ptr) & r->mask) > 0) {
    uint8_t c = CC_ACCESS_NOW(uint8_t, r->data[r->get_ptr]);
    return c;
  } else {
    return -1;
  }
}

void MicroIPUDP::flush()
{
  //???
}

int MicroIPUDP::raw_read()
{
  return ringbuf_get(&rxbuf);
}

int MicroIPUDP::raw_read(unsigned char* buf, size_t size)
{
  for(uint32_t i=0; i<size; i++) {
    int r = raw_read();
    if(r == -1) {
      return i;
    }
    buf[i] = (uint8_t)r;
  }
  return size;
}

void MicroIPUDP::receive(const uip_ipaddr_t *source_addr, uint16_t source_port,
               const uip_ipaddr_t *dest_addr, uint16_t dest_port,
               const uint8_t *data, uint16_t datalen)
{
  const uint16_t store_size =static_cast<uint16_t>(datalen+sz_pack);
  struct receive_pack pack = { store_size, *source_addr, source_port };
  PRINTF("MicroIPUDP::receive %d %d %d\n", sz_pack, store_size, datalen );

  if( store_size > ringbuf_size(&rxbuf) ) {
    //too large packet.
    return;
  }

  if( store_size > ringbuf_elements(&rxbuf) ) {
    PRINTF("MicroIPUDP::receive available() %d %d\n", available(), remaining );
    if( available() ) {
      //notify discard
      //discard current data
      while( available() ) {
        (void) raw_read();
        remaining--;
      }
    }
  }

  PRINTF("MicroIPUDP::receive ringbuf_size() %d ringbuf_elements() %d\n", ringbuf_size(&rxbuf) , ringbuf_elements(&rxbuf) );
  while( store_size < ringbuf_elements(&rxbuf) ) {
    // discard old packet.
    parsePacket();
  }

  for(int i=0; i<sz_pack; i++) {
    int r = ringbuf_put(&rxbuf, ((uint8_t*)&pack)[i]);
    if(r == 0) {
      break;
    }
  }

  for(int i=0; i<datalen; i++) {
    int r = ringbuf_put(&rxbuf, data[i]);
    if(r == 0) {
      break;
    }
  }
  PRINTF("MicroIPUDP::receive ringbuf_size() %d ringbuf_elements() %d\n", ringbuf_size(&rxbuf) , ringbuf_elements(&rxbuf) );

}

void MicroIPUDP::input_callback(struct udp_socket *c, void *ptr,
                               const uip_ipaddr_t *source_addr, uint16_t source_port,
                               const uip_ipaddr_t *dest_addr, uint16_t dest_port,
                               const uint8_t *data, uint16_t datalen)
{
  MicroIPUDP* udp = reinterpret_cast<MicroIPUDP*>(ptr);
  udp->receive(source_addr, source_port, dest_addr, dest_port, data, datalen);
}

void MicroIPUDP::do_udp_socket_register(void* ptr)
{
  struct udp_socket_params* params = reinterpret_cast<struct udp_socket_params*>(ptr);

  PRINTF("do_udp_socket_register\n");
  MicroIPUDP* udp = const_cast<MicroIPUDP*>(params->udp);
  params->retval = udp_socket_register(&udp->sock, udp, MicroIPUDP::input_callback);
}

void MicroIPUDP::do_udp_socket_bind(void* ptr)
{
  struct udp_socket_params* params = reinterpret_cast<struct udp_socket_params*>(ptr);

  PRINTF("do_udp_socket_bind\n");
  MicroIPUDP* udp = const_cast<MicroIPUDP*>(params->udp);
  params->retval = udp_socket_bind(&udp->sock, params->port);
}


void MicroIPUDP::do_udp_socket_connect(void* ptr)
{
  struct udp_socket_params* params = reinterpret_cast<struct udp_socket_params*>(ptr);

  int ret = udp_socket_connect(const_cast<udp_socket*>(&params->udp->sock),
                               const_cast<uip_ipaddr_t*>(params->ipaddr), params->port);
  PRINTF("return do_udp_socket_connect %d\n", ret);

  params->retval = ret;
}

void MicroIPUDP::do_udp_socket_sendto(void* ptr)
{
  struct udp_socket_params* params = reinterpret_cast<struct udp_socket_params*>(ptr);

  PRINTF("do_udp_socket_sendto\n");
  MicroIPUDP* udp = const_cast<MicroIPUDP*>(params->udp);
  PRINT6ADDR(params->ipaddr);
  PRINTF("\n");
  params->retval = udp_socket_sendto(&udp->sock, udp->tbuf, udp->txidx, params->ipaddr, params->port);
}
