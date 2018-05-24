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

#ifndef MICROIPUDP_H
#define MICROIPUDP_H

extern "C" {
#include <net/ip/udp-socket.h>
#include <lib/ringbuf.h>
}

#include <Udp.h>

#define UDP_TX_PACKET_MAX_SIZE 24

#ifndef MICROIPUDP_TXBUF_SIZE
#define MICROIPUDP_TXBUF_SIZE 128
#endif

#ifndef MICROIPUDP_RXBUF_SIZE
#define MICROIPUDP_RXBUF_SIZE UIP_BUFSIZE
#endif

class MicroIPUDP : public UDP {
private:
  IPAddress _remoteIP; 
  uint16_t _remotePort; 
  uint16_t remaining; 

  struct udp_socket sock;

  struct ringbuf rxbuf;
  uint8_t rbuf[MICROIPUDP_RXBUF_SIZE];
  uint8_t tbuf[MICROIPUDP_TXBUF_SIZE];
  uint32_t txidx;

public:
  MicroIPUDP();  
  virtual uint8_t begin(uint16_t);	
  virtual void stop();  
  virtual int beginPacket(IPAddress ip, uint16_t port);
  virtual int beginPacket(const char *host, uint16_t port);
  virtual int endPacket();
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buffer, size_t size);
  using Print::write;
  virtual int parsePacket();
  
  virtual int available();
  virtual int read();
  virtual int read(unsigned char* buffer, size_t len);
  virtual int read(char* buffer, size_t len) { return read((unsigned char*)buffer, len); };
  virtual int peek();
  virtual void flush();	
  
  virtual IPAddress remoteIP() { return _remoteIP; };
  virtual uint16_t remotePort() { return _remotePort; };

private:
  int raw_read();
  int raw_read(unsigned char* buffer, size_t len);

  void receive(const uip_ipaddr_t *source_addr, uint16_t source_port,
               const uip_ipaddr_t *dest_addr, uint16_t dest_port,
               const uint8_t *data, uint16_t datalen);


  static void input_callback(struct udp_socket *c, void *ptr,
                             const uip_ipaddr_t *source_addr, uint16_t source_port,
                             const uip_ipaddr_t *dest_addr, uint16_t dest_port,
                             const uint8_t *data, uint16_t datalen);

  static void do_udp_socket_register(void* ptr);
  static void do_udp_socket_bind(void* ptr);
  static void do_udp_socket_connect(void* ptr);
  static void do_udp_socket_send(void* ptr);
};

#endif
