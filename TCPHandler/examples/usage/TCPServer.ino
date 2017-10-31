// Example usage for TCPHandler library by Tomais Williamson.

#include "TCPServerHandler.h"

#define port 46290
#define internalLED D7

TCPServerHandler tcpSH = TCPServerHandler(port);

void setup() {
  Serial.begin(9600);
  //while (!Serial.available());
  pinMode(internalLED, OUTPUT);
}

void loop() {
  if (tcpSH.connected()) {

    if (tcpSH.messageAvailable()) {
      String message = String();

      if (tcpSH.tryReceiveMessage(message)) {
        Serial.print("Received message: ");
        Serial.println(message);
        digitalWrite(internalLED, HIGH);
        delay(200);
        digitalWrite(internalLED, LOW);

        if (strcmp(message, "ping")) {
          Serial.println("Replying with \"pong\"");
          tcpSH.sendMessage("Testeroni");
        }
      }
    }
  }
}
