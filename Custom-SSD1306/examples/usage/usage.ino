// Example usage for Custom_SSD1306 library by Tomais Williamson.

#include "Custom_SSD1306.h"

// Initialize objects from the lib
Custom_SSD1306 oled;

void setup() {
  oled.begin();
  oled.drawCenteredString(0, "This is a test");
  oled.display();
}

void loop() {

}
