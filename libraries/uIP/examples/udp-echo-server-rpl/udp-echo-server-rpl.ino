/*
  UDP Echo server

  A simple UDP communication demo.
  The program wait a message from 'udp-echo-client'.
  Send response message to the sender node on message received.
  The udp port 3000 uses to send client to server,
  the 3001 to send response.

  Even if RPL is enabled or disabled works.
  Use same settings to compile server side programs.
  This programs behavior is based on contiki os's
  example(examples/udp-ipv6/udp-server.c).

  created 24 Jan 2016
  modified 29 May 2018
  by Tokita Hiroshi
*/

#include <MicroIp.h>
#include <MicroIpUdp.h>
#include <IPAddress.h>

const size_t MAX_PAYLOAD_LEN = 40;
const char* HOSTNAME = "contiki-udp-server";
const char* PREFIX = "AAAA::0";
const unsigned short LOCAL_PORT = 3000;
const unsigned short DEST_PORT  = 3001;

MicroIPUDP Udp;

char packetBuffer[MAX_PAYLOAD_LEN];
long seq_id;

void setup() {
  Serial.begin(1000000);
  Serial.println("Start udp-echo-server-rpl");

  MicroIP.begin();

  // set hostname to mDNS announcing
  MicroIP.setHostname(HOSTNAME);

  // start RPL to find neighbors
  IPAddress rpl_prefix;
  rpl_prefix.fromString(PREFIX);
  RPL.begin(rpl_prefix);

  Udp.begin(LOCAL_PORT);
  Serial.print("Start listen port:");
  Serial.println(LOCAL_PORT);
}

void loop() {

  // if there's data available, read a packet
  if (int packetSize = Udp.parsePacket() ) {
    Udp.read(packetBuffer, packetSize);
    Serial.print("Server received(");
    Serial.print(packetSize);
    Serial.print("bytes): '");
    Serial.print(packetBuffer);
    Serial.print("' from ");
    Serial.print(Udp.remoteIP());
    Serial.println();

    // format message
    strcpy(packetBuffer, "Hello from the server! (");
    itoa(++seq_id, packetBuffer + strlen(packetBuffer), 10);
    strcpy(packetBuffer + strlen(packetBuffer), ")");

    Serial.print("Responding with message: ");
    Serial.println(packetBuffer);

    // send echo
    Udp.beginPacket(Udp.remoteIP(), DEST_PORT);
    Udp.write(packetBuffer, strlen(packetBuffer) );
    Udp.endPacket();
  }

}
