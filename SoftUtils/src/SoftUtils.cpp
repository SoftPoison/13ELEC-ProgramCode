#include "SoftUtils.h"

std::vector<String> separateMessage(String& message) {
  std::vector<String> strArray;
  strArray.reserve(10);

  int from = 0;
  int to = 0;
  while (from < message.length()) {
    to = message.indexOf(";", from);

    if (to == -1) {
      if (message.length() > 1)
        to = message.length();
      else
        break;
    }

    String sub = message.substring(from, to);

    if (sub.length() != 1)
        strArray.push_back(sub);

    from = to + 1;
  }

  strArray.shrink_to_fit();
  return strArray;
}

std::vector<uint8_t> getDirs(String &str) {
  std::vector<uint8_t> dirs;
  dirs.reserve(4);

  for (uint8_t i = 0; i < 4; i++)
    dirs[i] = str.substring(i, i + 1).toInt();

  return dirs;
}
