#include <MicroIp.h>
#include <MicroIpUdp.h>

IPAddress server(0xaaaa, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001);

unsigned int localPort = 7;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

MicroIPUDP Udp;

void setup() {
  Serial.begin(115200);
  MicroIP.begin();

  server = MicroIP.lookup("udp-echo-server.local");
  Udp.begin(localPort);
}

void loop() {

  int avail = Serial.available();
  if(avail) {
    int len = min(avail, UDP_TX_PACKET_MAX_SIZE);

    for(int i=0; i<len; i++) {
      packetBuffer[i] = Serial.read();
    }

    Udp.beginPacket(server, localPort); 
    Udp.write(packetBuffer, len);
    Udp.endPacket();
  }

  while(int packetSize = Udp.parsePacket()) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    Serial.print(remote);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    for(int i=0; i<packetSize; i++) {
      Serial.print(packetBuffer[i]);
    }
    Serial.println();
  }

}
