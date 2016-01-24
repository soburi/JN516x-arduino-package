#include <MicroIp.h>

#define BUFLEN 64

unsigned char buf[BUFLEN];

MicroIPServer server(7);
IPAddress prefix(0xaaaa, 0, 0, 0, 0, 0, 0, 0);

void setup() {
  Serial.begin(115200);
  Serial.println("Start Echo Server.");

  MicroIP.setHostname("echo-server");
  MicroIP.begin();

  Serial.print("Start RPL with prefix ");
  Serial.println(prefix);
  RPL.begin(prefix);
  
  delay(1000);

  server.begin();
}

void loop() {
  // API will be change...
  MicroIPClient& client = server.available();
  if(client) {
    if( client.connected() ) {
      Serial.println("connected");

      int count = 0;
      while(client.available() ) {
        buf[count] = client.read();
        count++;
        if(count == BUFLEN) break;
      }
      if(count > 0) {
        client.write(buf, count);
      }
    }
  }
}

