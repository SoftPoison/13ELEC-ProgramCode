// Example usage for TCPHandler library by Tomais Williamson.

#include "TCPClientHandler.h"

#define port 46290
#define internalLED D7

IPAddress address(192,168,1,10);

TCPClientHandler tcpCH = TCPClientHandler(address, port);

void sendPing() {
  Serial.println("Sending ping to server");
  //Send a message with the content "ping"
  tcpCH.sendMessage("ping");

  digitalWrite(internalLED, HIGH);
  delay(200);
  digitalWrite(internalLED, LOW);
}

//Ping the server every 10 seconds
Timer t1(10000, sendPing);

void setup() {
  Serial.begin(9600);
  while (!Serial.available());

  pinMode(internalLED, OUTPUT);

  tcpCH.connect();
  t1.start();
}

void loop() {
  if (tcpCH.connected()) {

    if (tcpCH.messageAvailable()) {
      String message = String();

      if (tcpCH.tryReceiveMessage(message)) {
        Serial.print("Received message: ");
        Serial.println(message);
      }
    }
  }
}
