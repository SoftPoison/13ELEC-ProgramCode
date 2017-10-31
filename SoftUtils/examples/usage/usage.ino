// Example usage for SoftUtils library by Tomais Williamson.

#include <vector>
#include "SoftUtils.h"

void setup() {
  String test = "dir;0123;";
  std::vector<String> parts = separateMessage(test);
}

void loop() {

}
