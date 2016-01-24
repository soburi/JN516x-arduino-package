#include <MicroIp.h>

IPAddress server(0xaaaa, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001);
MicroIPClient client;

void setup() {
  Serial.begin(115200);
  
  // start the uIP connection:
  Serial.println("MicroIP.begin");

  MicroIP.begin();
  delay(1000);

  // if you get a connection, report back via serial:
  Serial.println("connecting...\n");
  if (!client.connect(server, 80)) {
    // if you didn't get a connection to the server:
    Serial.println("connection failed.");

    while(true) { yield(); }
  }
  else {
    Serial.println("connected.");
    Serial.println("send GET request...");
    client.print("GET /\n");
  }
}

void loop() {
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
}
