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

#ifndef MICROIPCLIENT_H
#define MICROIPCLIENT_H

#include <stdint.h>

extern "C" {
#include <net/ip/tcp-socket.h>
#include <lib/ringbuf.h>
}

#include "Arduino.h"
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"

#ifndef MICROIPCLIENT_TXBUF_SIZE
#define MICROIPCLIENT_TXBUF_SIZE 128
#endif

#ifndef MICROIPCLIENT_RXBUF_SIZE
#define MICROIPCLIENT_RXBUF_SIZE UIP_BUFSIZE
#endif

class MicroIPClient : public Client {

public:
  MicroIPClient();

  uint8_t status();
  virtual int connect(IPAddress ip, uint16_t port);
  virtual int connect(const char *host, uint16_t port);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  virtual int available();
  virtual int read();
  virtual int read(uint8_t *buf, size_t size);
  virtual int peek();
  virtual void flush();
  virtual void stop();
  virtual uint8_t connected();
  virtual operator bool();

  friend class MicroIPServer;
  
  using Print::write;

private:
  MicroIPClient(struct tcp_socket* psock);
  size_t receive(const uint8_t *input_data_ptr, int input_data_len);

  int listen(uint16_t port);

  static void do_tcp_socket_register(void* pthis);
  static void do_tcp_socket_connect(void* pthis);
  static void do_tcp_socket_send(void* ptr);
  static void do_tcp_socket_listen(void* ptr);
  static void do_tcp_socket_close(void* ptr);

  static  int wait_tcp_socket_event(process_event_t ev, process_data_t data, void* param);

  static  int  data_callback(struct tcp_socket *s, void *ptr, const unsigned char* input_data_ptr, int input_data_len);
  static void event_callback(struct tcp_socket *s, void *ptr, tcp_socket_event_t event);

  struct tcp_socket* _psock;
  struct tcp_socket sock;
  struct ringbuf rxbuf;
  uint8_t rbuf[MICROIPCLIENT_RXBUF_SIZE];

  uint8_t socket_rxbuf[4];
  uint8_t socket_txbuf[MICROIPCLIENT_TXBUF_SIZE];

  tcp_socket_event_t state;

  bool event_wait;
};

#endif
