#include <MicroIp.h>
#include <MicroIpUdp.h>

IPAddress prefix(0xaaaa, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);

unsigned int localPort = 7;

MicroIPUDP Udp;

void setup() {
  Serial.begin(115200);

  RPL.begin(prefix);
  MicroIP.setHostname("udp-echo-server");
  MicroIP.begin();

  Udp.begin(localPort);
}

void loop() {
  
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    Serial.print(remote);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    while(Udp.available() ) {
      char c = Udp.read();
      Udp.write(c);
      Serial.print(c);
    }
    Udp.endPacket();
    Serial.println();
  }
  delay(10);

}

