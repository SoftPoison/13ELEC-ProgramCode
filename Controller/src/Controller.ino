/*
 * Project Controller
 * Description: Source code for the controller portion of my project
 * Author: Tomais Williamson
 * Date: 15/05/17
 */

//Dependancies
#include <vector>
#include <Custom_SSD1306.h>
#include <SoftUtils.h>
#include <TCPServerHandler.h>

//Constants
#define pinHori A3
#define pinVert A4
#define pinButtonUp A0
#define pinButtonA A1
#define pinButtonDown A2
#define TCPport 46290

#define minDir 0
#define maxDir 4095
#define medDir 2047

#define strf String::format

#define DEBUG_MODE

//Variables
Custom_SSD1306 oled = Custom_SSD1306();
TCPServerHandler tcpSH = TCPServerHandler(TCPport);
bool splashIdle = false;

uint8_t lastLeft, lastRight, lastUp, lastDown = 0;
uint8_t directions = 0;

uint16_t minHorizontal, minVertical = maxDir;
uint16_t maxHorizontal, maxVertical = minDir;
uint16_t medHorizontal, medVertical = medDir;

SYSTEM_MODE(SEMI_AUTOMATIC);

//Forward declarations

bool calibrate();
char* dirsToChars();
bool handleJoystick();

/** Device setup

  Setup is run once on device boot, allowing for device and peripheral
  initialisation
*/
void setup() {
  pinMode(pinButtonA, INPUT_PULLDOWN);

  oled.begin();

  oled.clearString(0);
  oled.drawCenteredString(0, "Press A when done");
  oled.drawMessage(Message::calibrationMode);
  oled.display();

  while (!calibrate());

  oled.drawMessage(Message::wifiConnecting);
  oled.display();

  WiFi.on();
  WiFi.setCredentials("Disconnectify", "StrongPassword", WPA2);
  WiFi.connect();

  while (!WiFi.ready()); //Wait for a WiFi connection

  oled.drawMessage(Message::waitingForClient);
  oled.display();
}

/** Device main loop

  Loops over and over again until device shutdown. First execution occurs
  immediately after termination of `setup`
*/
void loop() {
  if (tcpSH.connected()) {
    if (oled.drawMessage(Message::connected)) {
      oled.clearString(0);
      oled.clearString(8);
      oled.drawCenteredString(0, "Idle");
      oled.display();
    }

    if (tcpSH.messageAvailable()) {
      String message = String();

      oled.clearString(0);
      oled.drawCenteredString(0, "Receiving data");
      oled.display();
      splashIdle = true;

      if (tcpSH.tryReceiveMessage(message)) { //Test code for now
        oled.clearString(8);
        oled.drawCenteredString(8, message);
        oled.display();
      }
    }
    else { //If there isn't a message available
      if (splashIdle) {
        oled.clearString(0);
        oled.drawCenteredString(0, "Idle");
        oled.display();
        splashIdle = false;
      }
    }

    if (handleJoystick()) {
      char* dirs = dirsToChars();

      tcpSH.sendMessage(strf("dir;%c%c%c%c;", dirs[0], dirs[1], dirs[2], dirs[3]));
    }
  }
  else {
    //If there's no client connected
    if (oled.drawMessage(Message::waitingForClient)) {
      oled.clearString(0);
      oled.clearString(8);
      oled.display();
    }
  }
}

bool calibrate() {
  uint16_t horizontal = analogRead(pinHori);
  uint16_t vertical = analogRead(pinVert);

  if (horizontal < minHorizontal) minHorizontal = horizontal;
  else if (horizontal > maxHorizontal) maxHorizontal = horizontal;
  if (vertical < minVertical) minVertical = vertical;
  else if (vertical > maxVertical) maxVertical = vertical;

  oled.clearString(0);
  oled.drawString(0, 0, strf("H: %d|%d|%d", horizontal, minHorizontal, maxHorizontal));
  oled.clearString(8);
  oled.drawString(0, 8, strf("V: %d|%d|%d", vertical, minVertical, maxVertical));
  oled.display();

  bool buttonPressed = digitalRead(pinButtonA);

  if (buttonPressed) {
    oled.clearString(0);
    oled.clearString(8);
    oled.display();

    medHorizontal = (maxHorizontal - minHorizontal) / 2;
    medVertical = (maxVertical - minVertical) / 2;
  }

  return buttonPressed;
}

/** Turns directions into characters

  Turns the internal `directions` variable into 4 characters

  @returns a char array of the 4 direction values
*/
char* dirsToChars() {
  char* out = new char[5]; //Always need a null character at the end
  sprintf(out, "%x%x%x%x", (directions >> 6 & 3), (directions >> 4 & 3),
                           (directions >> 2 & 3), (directions      & 3));

  return out;
}

/** Handles input from joystick

  Gets input from the joystick and smooths it so it only returns true on a
  "dramatic" change in input

  @returns true if the joysticks directions have changed dramatically
*/
bool handleJoystick() {
  const uint16_t SMOOTHING_AMOUNT = 50;
  uint16_t horizontalGranularity = medHorizontal / 8; //Gives output between [0 .. 3]
  uint16_t verticalGranularity = medVertical / 8;

  uint16_t horizontal = analogRead(pinHori);
  uint16_t vertical = analogRead(pinVert);

  int16_t left = max(medHorizontal - horizontal, 0);
  int16_t right = max(horizontal - medHorizontal, 0);
  int16_t up = max(vertical - medVertical, 0);
  int16_t down = max(medVertical - vertical, 0);

  #ifdef DEBUG_MODE
    oled.clearString(8);
    oled.drawCenteredString(8, strf("H: %d; V: %d", horizontal, vertical));
    oled.display();
  #endif

  //Basically just software implemented smoothing
  if (abs(left - lastLeft)   < SMOOTHING_AMOUNT &&
      abs(right - lastRight) < SMOOTHING_AMOUNT &&
      abs(up - lastUp)       < SMOOTHING_AMOUNT &&
      abs(down - lastDown)   < SMOOTHING_AMOUNT)
    return false;

  uint8_t dirs = (((left  / horizontalGranularity) << 6)
               +  ((right / horizontalGranularity) << 4)
               +  ((up    / verticalGranularity) << 2)
               +  ((down  / verticalGranularity)     ));

  if (dirs != directions) {
    directions = dirs;
    lastLeft = left;
    lastRight = right;
    lastUp = up;
    lastDown = down;

    return true;
  }

  return false;
}
