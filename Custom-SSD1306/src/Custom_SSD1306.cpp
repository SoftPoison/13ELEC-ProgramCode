#include "Custom_SSD1306.h"

Custom_SSD1306::Custom_SSD1306() : Adafruit_SSD1306(-1) { }

void Custom_SSD1306::begin() {
  Wire.setSpeed(400000);

  Adafruit_SSD1306::begin();
}

void Custom_SSD1306::clearString(uint16_t y) {
  this->fillRect(0, y, 128, 7, 0);
}

void Custom_SSD1306::drawString(uint16_t x, uint16_t y, String str, uint16_t fgColor,
                 uint16_t bgColor, uint8_t fontSize) {
  this->setCursor(x, y);
  for (uint16_t i = 0; i < str.length(); i++)
    this->drawChar(x + 5 * i * fontSize + i, y, str.charAt(i), fgColor, bgColor, fontSize);
}

void Custom_SSD1306::drawString(uint16_t x, uint16_t y, String str) {
  this->drawString(x, y, str, 1, 0, 1);
}

void Custom_SSD1306::drawCenteredString(uint16_t y, String str) {
  uint16_t x = 64 - str.length() * 3;
  this->drawString(x, y, str, 1, 0, 1);
}

//Draws the message in the center at the bottom of the display
bool Custom_SSD1306::drawMessage(Message message, bool force) {
  if (message != this->lastMessage || force) {
    this->clearString(56);
    this->drawCenteredString(56, Messages[message]);
    this->lastMessage = message;

    return true;
  }

  return false;
}
