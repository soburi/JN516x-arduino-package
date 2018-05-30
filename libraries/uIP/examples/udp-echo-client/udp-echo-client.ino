/*
  UDP Echo client

  A simple UDP communication demo.
  The program send simple message to 'udp-echo-server-rpl' node,
  the server node send echo on the message received.
  The received echo message shows on serial port.

  Even if RPL is enabled or disabled works.
  Use same settings to compile server side programs.
  This programs behavior is based on contiki os's
  example(examples/udp-ipv6/udp-client.c).

  created 24 Jan 2016
  modified 29 May 2018
  by Tokita Hiroshi
*/

#include <MicroIp.h>
#include <MicroIpUdp.h>
#include <IPAddress.h>

const size_t MAX_PAYLOAD_LEN = 40;
const char* UDP_CONNECTION_ADDR = "contiki-udp-server.local";
const unsigned short LOCAL_PORT = 3001;
const unsigned short DEST_PORT  = 3000;
const int INTERVAL = 15;

MicroIPUDP Udp;
IPAddress server;

char packetBuffer[MAX_PAYLOAD_LEN];
long lastsend;
long seq_id;

void setup() {
  Serial.begin(1000000);
  Serial.println("Start udp-echo-cleint");
  MicroIP.begin();

  server = MicroIP.lookup(UDP_CONNECTION_ADDR);

  if (server == IN6ADDR_ANY_INIT) {
    Serial.print("Server [");
    Serial.print(UDP_CONNECTION_ADDR);
    Serial.println("] is not found.");
    while (true) {
      yield();
    }
  }

  Serial.print("Server ");
  Serial.print(UDP_CONNECTION_ADDR);
  Serial.print(" is [");
  Serial.print(server);
  Serial.println("]");

  Udp.begin(LOCAL_PORT);
  Serial.print("Start listen port:");
  Serial.println(LOCAL_PORT);
}

void loop() {

  long now = millis();

  // Periodically send.
  if ((now - lastsend) > (INTERVAL * 1000)) {
    // format message
    memset(packetBuffer, 0, MAX_PAYLOAD_LEN);
    strcpy(packetBuffer, "Hello ");
    itoa(++seq_id, packetBuffer + strlen(packetBuffer), 10);
    strcpy(packetBuffer + strlen(packetBuffer), " from the client");

    Serial.print("Client sending to ");
    Serial.print(server);
    Serial.print(" (msg: ");
    Serial.print(packetBuffer);
    Serial.println(");");

    // send packet
    Udp.beginPacket(server, DEST_PORT);
    Udp.write(packetBuffer, strlen(packetBuffer));
    Udp.endPacket();

    lastsend = now;
  }

  while (int packetSize = Udp.parsePacket()) {
    /*
      //more info
      Serial.print("Receive from ");
      Serial.print(Udp.remoteIP());
      Serial.print(":");
      Serial.print(Udp.remotePort());
      Serial.print(" size:");
      Serial.println(packetSize);
    */

    // read the packet into packetBufffer
    Udp.read(packetBuffer, packetSize);
    Serial.print("Response from the server: '");
    Serial.print(packetBuffer);
    Serial.println("'");
  }

}
