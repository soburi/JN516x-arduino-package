#include <MicroIp.h>

#define BUFLEN 64

unsigned char buf[BUFLEN];

MicroIPClient client;

void setup() {
  Serial.begin(115200);
  Serial.println("Start Echo Client.");

  MicroIP.begin();
  delay(1000);
}

void loop() {
  if( !client.connected() ) {
    int ret = client.connect("echo-server.local", 7);
    Serial.print("connect ");
    Serial.println(ret);
    delay(1000);
  }
  else {
    int count = 0;
    while(client.available() ) {
      buf[count] = client.read();
      count++;
      if(count == 64) break;
    }
    if(count > 0) {
      Serial.write(buf, count);
    }
    
    count = 0;
    while(Serial.available() ) {
      buf[count] = Serial.read();
      count++;
      if(count == BUFLEN) break;
    }
    if(count > 0) {
      client.write(buf, count);
    }
  }
}
