#include <JN_AN_1178.h>

int destaddr = 0xFFFF;
char buf[128];

void onWUartAssociated(int id)
{
    Serial.print("onWUartAssociated:");
    Serial.println(id, HEX);
    destaddr = id;
}

void setup() {
  Serial.begin(115200);
  Serial.println("setup");
    
  JN_AN_1178.init(0x0a55U);
  JN_AN_1178.startCoordinator(0x00000800UL);
}

void loop() {
  if(destaddr != 0xFFFF) {
    int len = Serial.available();
    len= len > 64 ? 64 :len;
    
    for(int i=0; i<len; i++) {
      buf[i] = Serial.read();
    }
    
    if(len > 0) {
      JN_AN_1178.WUART(destaddr).write(buf, len);
    }
    
    while( JN_AN_1178.WUART(destaddr).available() ) {
      Serial.write( JN_AN_1178.WUART(destaddr).read() );
    }
  }

  delay(10);
}

