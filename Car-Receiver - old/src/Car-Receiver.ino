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
#include <TCPClientHandler.h>

/* Definitions of internal constants */
#define TCPport 46290

#define buttonUp A2
#define buttonSelect A1
#define buttonDown A0

#define pinForward A4
#define pinBackward A5
#define pinLeft D2
#define pinRight D3

//Removes the need to preprend `vector` with `std::`
using namespace std;

//Singleton instance of a TCPClientHandler which connects to the remote
TCPClientHandler tcpCH = TCPClientHandler(nullptr, TCPport);
Custom_SSD1306 oled = Custom_SSD1306();

//Forward function declarations
bool controllerSearch(uint8_t& lastByte);

//Stops the Photon from trying to connect to the cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

/** Device setup

  Setup is run once on device boot, allowing for device and peripheral
  initialisation
*/
void setup() {
  oled.begin();
  oled.drawMessage(Message::wifiConnecting);
  oled.display();

  WiFi.on();
  WiFi.setCredentials("Disconnectify", "StrongPassword", WPA2);
  WiFi.connect();
  while (!WiFi.ready());

  oled.drawMessage(Message::wifiConnected);
  oled.display();
  delay(1000);

  pinMode(buttonUp, INPUT_PULLDOWN);
  pinMode(buttonSelect, INPUT_PULLDOWN);
  pinMode(buttonDown, INPUT_PULLDOWN);

  pinMode(pinForward, OUTPUT);
  pinMode(pinBackward, OUTPUT);
  pinMode(pinLeft, OUTPUT);
  pinMode(pinRight, OUTPUT);
  
  oled.drawMessage(Message::controllerSearching);
  uint8_t lastByte = 0;
  while (!controllerSearch(lastByte));
}

/** Device main loop

  Loops over and over again until device shutdown. First execution occurs
  immediately after termination of `setup`
*/
void loop() {
  if (tcpCH.connected()) {
    //Device is connected
    if (oled.drawMessage(Message::connected))
      oled.display();

    if (tcpCH.messageAvailable()) {
      //Message is available

      String message;

      if (tcpCH.tryReceiveMessage(message)) {
        //Message successfully received
        oled.clearString(8);
        oled.drawCenteredString(8, message);
        oled.display();

        vector<String> cmdAndArgs = separateMessage(message);
        if (cmdAndArgs.size() == 0)
          //The message could not be split successfully or trash message
          return;

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
      }
    }
    else {
      //Normal stuff when there is no message available
    }
  }
  else {
    //No device connected
    if (oled.drawMessage(Message::controllerSearching)) {
      oled.clearString(8);
      oled.display();
	  uint8_t lastByte = 0;
	  while (!controllerSearch(lastByte));
	}
  }
}

bool controllerSearch(uint8_t& lastByte) {
  delay(100);

  if (digitalRead(buttonUp) || digitalRead(buttonSelect) || digitalRead(buttonDown)) {
	IPAddress ip = WiFi.localIP();
    ip[3] = lastByte;
	
	oled.clearString(0);
	oled.drawCenteredString(0, String(WiFi.localIP()));
	
    if (digitalRead(buttonSelect)) {
      if (WiFi.ping(ip)) {
        tcpCH = TCPClientHandler(ip, TCPport);

        if (tcpCH.connected())
          return true; //Connected
      }
    }
    else if (digitalRead(buttonUp)) //Increase button
      lastByte++;
    else if (digitalRead(buttonDown)) //Decrease button
      lastByte--;
  }

  return false; //Not yet connected
}
