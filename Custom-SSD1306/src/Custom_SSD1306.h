#ifndef _CUSTOM_SSD1306_H
#define _CUSTOM_SSD1306_H

#include "application.h"
#include "Adafruit_SSD1306.h"

#define CHARACTER_WIDTH 5
#define CHARACTER_HEIGHT 7

enum Message {
  null,
  wifiConnecting,
  wifiConnected,
  calibrationMode,
  controllerSearching,
  waitingForClient,
  connected,
};

const String Messages[] = {
  "",
  "Connecting to WiFi",
  "Connected to WiFi",
  "Calibration mode",
  "Looking for remote",
  "Waiting for client",
  "Connected",
};

class Custom_SSD1306 : public Adafruit_SSD1306 {
public:
  Custom_SSD1306();

  void begin();

  void clearString(uint16_t y);

  void drawString(uint16_t x, uint16_t y, String str, uint16_t fgColor,
                             uint16_t bgColor, uint8_t fontSize);
  void drawString(uint16_t x, uint16_t y, String str);
  void drawCenteredString(uint16_t y, String str);

  bool drawMessage(Message message, bool force = false);

private:
  Message lastMessage;
};

#endif // _CUSTOM_SSD1306_H
