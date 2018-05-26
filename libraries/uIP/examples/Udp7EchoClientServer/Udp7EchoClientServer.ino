/*
  Udp7EchoClientServer:

  This is a client / server program which communicate 
  by "well-known udp/7" echo protocol.
  
  Write same program to 2 nodes to use.
  One is act as server, the other is client.
  
  Program is start as echo server at boot up.
  Server send response same data what if data received. 
  Enter the IPv6 address of the server to serial,
  and act as a client when the address is recognized.

  This program must compile with [RPL: "disabled"] configuration.

  created 29 May 2018
  by Tokita Hiroshi
*/

#include <MicroIp.h>
#include <MicroIpUdp.h>

IPAddress destAddr;

const size_t MAX_PAYLOAD_LEN = 33;
const size_t MAX_V6_ADDR_STR = 40;

const unsigned int ECHO_PORT = 7;

char packetBuffer[MAX_PAYLOAD_LEN];
char serialBuffer[MAX_V6_ADDR_STR];
size_t serialBufferLen;

MicroIPUDP Udp;

void setup() {
  Serial.begin(1000000);

  Serial.print("Echo protocol (");
  Serial.print(ECHO_PORT);
  Serial.println("/udp) listen start.");
  Serial.println();

  MicroIP.begin();

  // Show link-local address
  Serial.print("This linklocal Address is ");
  Serial.println(MicroIP.linklocalAddress());
  Serial.println();

  Udp.begin(ECHO_PORT);

  Serial.println();
  Serial.println("Enter destination IP address.");
  Serial.println("Or invalid IP address is entered, stop sending.");
  Serial.println();
}

void loop() {

  if (Serial.available())
  {
    memset(serialBuffer, 0, MAX_V6_ADDR_STR);
    serialBufferLen = Serial.readBytes(serialBuffer, MAX_V6_ADDR_STR);

    // Set destination IPaddress when string received from serial line.
    if (destAddr == IN6ADDR_ANY_INIT)
    {
      bool valid = destAddr.fromString(serialBuffer);
      if (valid)
      {
        Serial.print("Destination IP is ");
        Serial.println(destAddr);
        Serial.println("Enter message to send.");
      }
      else
      {
        Serial.print(serialBuffer);
        Serial.println(" is invalid. Stop sending.");
      }
      serialBufferLen = 0;
    }
  }

  // When destination is set, send serial buffer 
  if (!(destAddr == IN6ADDR_ANY_INIT) && serialBufferLen != 0)
  {
    // construct send data
    memcpy(packetBuffer, serialBuffer, serialBufferLen);

    Serial.print("Send to [");
    Serial.print(destAddr);
    Serial.print("]:");
    Serial.print(ECHO_PORT);
    Serial.print(" :");
    Serial.println(serialBuffer);

    // send packet
    Udp.beginPacket(destAddr, ECHO_PORT);
    Udp.write(serialBuffer, min(MAX_PAYLOAD_LEN, serialBufferLen) );
    Udp.endPacket();

    serialBufferLen = 0;
  }

  // print received data.
  while (int packetSize = Udp.parsePacket()) {
    Serial.print("Received packet from [");
    Serial.print(Udp.remoteIP());
    Serial.print("]:");
    Serial.print(Udp.remotePort());
    Serial.print(" datasize=");
    Serial.println(packetSize);

    // Read the packet into packetBufffer
    memset(packetBuffer, 0, MAX_PAYLOAD_LEN);
    Udp.read(packetBuffer, MAX_PAYLOAD_LEN);

    if (destAddr == Udp.remoteIP()) {
      // Not response to echo.
      Serial.print("Received: ");
      Serial.println(packetBuffer);
    }
    else {
      // Send echo
      Serial.print("Echo: ");
      Serial.println(packetBuffer);
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.write(packetBuffer, packetSize);
      Udp.endPacket();
    }
  }

}
