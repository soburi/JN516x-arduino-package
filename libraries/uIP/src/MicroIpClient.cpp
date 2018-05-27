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
#include "wiring_private.h"
#include "MicroIp.h"
#include "MicroIpClient.h"

struct tcp_socket_params {
  MicroIPClient* client;
  const uip_ipaddr_t* ipaddr;
  uint16_t port;
  int retval;
};

struct tcp_send_params {
  const MicroIPClient* client;
  const uint8_t* buf;
  size_t size;
  int retval;
};


MicroIPClient::MicroIPClient() {
  state = TCP_SOCKET_CLOSED;
  _psock = &sock;
  ringbuf_init(&rxbuf, rbuf, static_cast<uint8_t>(MICROIPCLIENT_RXBUF_SIZE) );
}

MicroIPClient::MicroIPClient(struct tcp_socket* psock) {
  _psock = psock;
  ringbuf_init(&rxbuf, rbuf, static_cast<uint8_t>(MICROIPCLIENT_RXBUF_SIZE) );
}

int MicroIPClient::connect(const char* host, uint16_t port) {
  PRINTF("MicroIPClient::connect(%s, %d)\n", host, port);
  IPAddress remote = MicroIP.lookup(host);
  if(remote == IN6ADDR_ANY_INIT) {
    return 0;
  }
  return connect(remote, port);
}

int MicroIPClient::connect(IPAddress ip, uint16_t port) {
  PRINTF("MicroIPClient::connect(ip,port)\n");
  struct tcp_socket_params params = { this, this->uipIPAddress(ip), port, 0 };

  yield_continue(do_tcp_socket_register, &params);

  PRINTF("MicroIPClient::connect(ip,port) reg_params.retval=%d\n", params.retval);
  if(params.retval != 1)
  {
    return 0;
  }

  params.retval = 0;
  yield_until(do_tcp_socket_connect, &params, wait_tcp_socket_event, &params);

  PRINTF("return MicroIPClient::connect(ip,port) params.retval=%d\n", params.retval);
  if(params.retval != 1)
  {
    return 0;
  }

  return (state == 0);
}

size_t MicroIPClient::write(uint8_t b) {
  return write(&b, 1);
}

size_t MicroIPClient::write(const uint8_t *buf, size_t size) {
  PRINTF("MicroIPClient::write(%x,%d)\n", buf, size);

  struct tcp_send_params params = { this, buf, size, 0 };
  //yield_continue(do_tcp_socket_send, &params);
  yield_until(do_tcp_socket_send, &params, wait_tcp_socket_event, &params);

  PRINTF("return MicroIPClient::write %d\n", params.retval);
  if (!params.retval) {
    setWriteError();
    return 0;
  }
  return params.retval;
}

int MicroIPClient::available() {
  int ret = ringbuf_elements(&rxbuf);
  //PRINTF("available %d\n", ret);
  return ret;
}

int MicroIPClient::read() {
  return ringbuf_get(&rxbuf);
}

int MicroIPClient::read(uint8_t *buf, size_t size) {
  for(uint32_t i=0; i<size; i++) {
    int r = ringbuf_get(&rxbuf);
    if(r == -1) {
      return i;
    }
    buf[i] = (uint8_t)r;
  }
  return size;
}

int MicroIPClient::peek() {
  struct ringbuf *r = &rxbuf;
  if(((r->put_ptr - r->get_ptr) & r->mask) > 0) {
    uint8_t c = CC_ACCESS_NOW(uint8_t, r->data[r->get_ptr]);
    return c;
  } else {
    return -1;
  }
}

void MicroIPClient::flush() {
  // Nothing to do.
}

void MicroIPClient::do_tcp_socket_close(void* ptr)
{
  struct tcp_socket_params* params = reinterpret_cast<struct tcp_socket_params*>(ptr);

  PRINTF("do_tcp_socket_close\n");
  const_cast<MicroIPClient*>(params->client)->event_wait = true;

  int ret = tcp_socket_close(&params->client->sock);
  PRINTF("return do_tcp_socket_close %d\n", ret);
  if(ret == -1) {
    params->retval = ret;
    post_continue();
  }
}

void MicroIPClient::stop() {
  PRINTF("MicroIPClient::stop()\n");

  struct tcp_socket_params params = { this, NULL, 0, 0 };

  yield_until(do_tcp_socket_close, &params, wait_tcp_socket_event, &params);

  if(params.retval != 1)
  {
    PRINTF("error: tcp_socket_close params.retval=%d\n", params.retval);
  }
}

uint8_t MicroIPClient::connected() {
  uint8_t connected_ = (state == TCP_SOCKET_CONNECTED || state == TCP_SOCKET_DATA_SENT);
  return connected_;
}

uint8_t MicroIPClient::status() {
  return state;
}

// the next function allows us to use the client returned by
// MicroIPServer::available() as the condition in an if-statement.

MicroIPClient::operator bool() {
  return (_psock != NULL);
}

size_t MicroIPClient::receive(const uint8_t *input_data_ptr, int input_data_len)
{
  for(int i=0; i<input_data_len; i++) {
    int r = ringbuf_put(&rxbuf, input_data_ptr[i]);
    if(r == 0) {
      return i;
    }
  }
  return input_data_len;
}


void MicroIPClient::do_tcp_socket_register(void* ptr)
{
  struct tcp_socket_params* params = reinterpret_cast<struct tcp_socket_params*>(ptr);

  PRINTF("do_tcp_socket_register\n");
  MicroIPClient* client = const_cast<MicroIPClient*>(params->client);
  params->retval = tcp_socket_register(&client->sock, client,
		  client->socket_rxbuf, 4, client->socket_txbuf, MICROIPCLIENT_TXBUF_SIZE,
		  MicroIPClient::data_callback, MicroIPClient::event_callback);
}

void MicroIPClient::do_tcp_socket_connect(void* ptr)
{
  struct tcp_socket_params* params = reinterpret_cast<struct tcp_socket_params*>(ptr);

  PRINTF("do_tcp_socket_connect\n");
  const_cast<MicroIPClient*>(params->client)->event_wait = true;

  int ret = tcp_socket_connect(&params->client->sock, params->ipaddr, params->port);
  PRINTF("return do_tcp_socket_connect %d\n", ret);
  if(ret == -1) {
    params->retval = ret;
    post_continue();
  }
}

int MicroIPClient::wait_tcp_socket_event(process_event_t ev, process_data_t data, void* ptr) {
  struct tcp_socket_params* params = reinterpret_cast<struct tcp_socket_params*>(ptr);
  MicroIPClient* client = const_cast<MicroIPClient*>(params->client);
  (void)ev;
  (void)data;

  PRINTF("wait_tcp_socket_event client->state=%d\n", client->state);
  if(client->event_wait) {
    return 0;
  }

  params->retval = (client->state == 0);
  return 1;
}

void MicroIPClient::do_tcp_socket_send(void* ptr) {
  struct tcp_send_params* params = reinterpret_cast<struct tcp_send_params*>(ptr);
  MicroIPClient* client = const_cast<MicroIPClient*>(params->client);
  const_cast<MicroIPClient*>(params->client)->event_wait = true;

  PRINTF("call tcp_socket_send\n");
  //params->retval = tcp_socket_send(client->_psock, params->buf, params->size);
  int ret  = tcp_socket_send(&client->sock, params->buf, params->size);
  PRINTF("return do_tcp_socket_send %d\n", ret);
  if(ret == -1) {
    params->retval = ret;
    post_continue();
  }
}

void MicroIPClient::do_tcp_socket_listen(void* ptr)
{
  struct tcp_socket_params* params = reinterpret_cast<struct tcp_socket_params*>(ptr);

  PRINTF("do_tcp_socket_listen %d\n", params->port);
  params->retval = tcp_socket_listen(&params->client->sock, params->port);
}

int MicroIPClient::listen(uint16_t port)
{
  PRINTF("MicroIPClient::listen(%d)\n", port);
  struct tcp_socket_params reg_params = { this, NULL, 0, 0 };

  yield_continue(do_tcp_socket_register, &reg_params);

  PRINTF("MicroIPClient::listen(port) reg_params.retval=%d\n", reg_params.retval);
  if(reg_params.retval != 1)
  {
    return 0;
  }

  struct tcp_socket_params params = { this, NULL, port, 0 };
  yield_continue(do_tcp_socket_listen, &params);

  PRINTF("return MicroIPClient::listen(port) params.retval=%d\n", params.retval);
  if(params.retval != 1)
  {
    return 0;
  }

  return 1;
}

int MicroIPClient::data_callback(struct tcp_socket *s, void *ptr, const uint8_t *input_data_ptr, int input_data_len) {
  PRINTF("MicroIPClient::data_callback %d\n", input_data_len);
  MicroIPClient* client = reinterpret_cast<MicroIPClient*>(ptr);
  (void)s;
  int wb = client->receive(input_data_ptr, input_data_len);
  return (input_data_len - wb);
}

void MicroIPClient::event_callback(struct tcp_socket *s, void *ptr, tcp_socket_event_t event) {
  PRINTF("MicroIPClient::event_callback %d\n", event);
  MicroIPClient* client = reinterpret_cast<MicroIPClient*>(ptr);
  (void)s;
  client->state = event;
  client->event_wait= false;
  post_continue();
}
