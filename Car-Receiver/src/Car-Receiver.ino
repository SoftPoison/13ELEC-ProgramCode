/*
 * Project Car-Receiver
 * Description: The receiver/actual car portion of this project
 * Author: Tomais Williamson
 * Date: 07/06/17
 */

/* Project Dependancies */
#include <vector>
#include <Custom_SSD1306.h>
#include <SoftUtils.h>
#include <TCPServerHandler.h>

/* Definitions of internal constants */
#define TCPport 46290

#define buttonUp A2
#define buttonSelect A1
#define buttonDown A0

#define pinForward D2
#define pinBackward D3
#define pinLeft A4
#define pinRight A5

//Removes the need to preprend `vector` with `std::`
using namespace std;

TCPServerHandler tcpSH = TCPServerHandler(TCPport);
Custom_SSD1306 oled = Custom_SSD1306();

//Forward function declarations
void controllerSearch();

//Stops the Photon from trying to connect to the cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

/** Device setup

  Setup is run once on device boot, allowing for device and peripheral
  initialisation
*/
void setup() {
  Serial.begin();
  //while (!Serial.available());
  Serial.println("Debugger attatched");

  Serial.println("Initialising OLED");
  oled.begin();
  oled.drawMessage(Message::wifiConnecting);
  oled.display();

  Serial.println("Initialising WiFi");
  WiFi.on();
  WiFi.setCredentials("Disconnectify", "StrongPassword", WPA2);
  WiFi.connect();
  Serial.println("Waiting for WiFi...");
  while (!WiFi.ready());
  Serial.println("WiFi connected");

  oled.drawMessage(Message::wifiConnected);
  oled.display();
  delay(1000);

  Serial.println("Waiting for client to connect");
  oled.clearString(0);
  oled.drawCenteredString(0, String(WiFi.localIP()));
  oled.drawMessage(Message::waitingForClient);
  oled.display();

  Serial.println("Initialising inputs");
  pinMode(buttonUp, INPUT_PULLDOWN);
  pinMode(buttonSelect, INPUT_PULLDOWN);
  pinMode(buttonDown, INPUT_PULLDOWN);

  Serial.println("Initialising outputs");
  pinMode(pinForward, OUTPUT);
  pinMode(pinBackward, OUTPUT);
  pinMode(pinLeft, OUTPUT);
  pinMode(pinRight, OUTPUT);
}

/** Device main loop

  Loops over and over again until device shutdown. First execution occurs
  immediately after termination of `setup`
*/
void loop() {
  if (tcpSH.connected()) {
    //Device is connected
    if (oled.drawMessage(Message::connected)) {
      Serial.println("Client connected");
      oled.display();
    }

    if (tcpSH.messageAvailable()) {
      //Message is available
      Serial.println("Message available");

      String message;

      if (tcpSH.tryReceiveMessage(message)) {
        //Message successfully received
        Serial.print("Message received: ");
        Serial.println(message);
        oled.clearString(8);
        oled.drawCenteredString(8, message);
        oled.display();

        vector<String> cmdAndArgs = separateMessage(message);
        if (cmdAndArgs.size() == 0) {
          //The message could not be split successfully or trash message
          Serial.println("Error: Message failed to parse");
          return;
        }

        Serial.println("Message successfully parsed");
        Serial.print("Command: ");
        Serial.println(cmdAndArgs[0]);
        if (cmdAndArgs[0].equals("dir") && cmdAndArgs.size() == 2) {
          vector<uint8_t> dirs = getDirs(cmdAndArgs[1]);
          digitalWrite(pinRight, dirs[0] > 0);
          digitalWrite(pinLeft, dirs[1] > 0);
          analogWrite(pinForward, dirs[2] * 63);
          analogWrite(pinBackward, dirs[3] * 63);
        }
      }
      else {
        //Message could not be received
        Serial.println("Error: Message could not be received successfully");
      }
    }
    else {
      //Normal stuff when there is no message available
    }
  }
  else {
    //No device connected
    if (oled.drawMessage(Message::waitingForClient)) {
      Serial.println("Waiting for client to connect");
      oled.clearString(8);
      oled.display();
    }
  }
}
