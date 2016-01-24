#include <MicroIp.h>

MicroIPServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("MicroIP.begin");
  
  MicroIP.begin();
  delay(1000);

  server.begin();
  Serial.println("Tiny HTTPd started.");
}

void loop() {
  // API will be change...
  MicroIPClient& client = server.available();
  if(client) {
    Serial.println("new client");

    bool respond = false;

    while( client.connected() ) {
      if (client.available() ) {
        char c = client.read();
        Serial.write(c);

        if(c == '\n' && !respond) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML><title>Tiny HTTPd</tile>");
          client.println("<html>");
          client.println("</html>");
          respond = true;
        }
      }
      else if(respond) {
        Serial.println("stop");
        client.stop();
      }
    }
    delay(1);
  }
}
