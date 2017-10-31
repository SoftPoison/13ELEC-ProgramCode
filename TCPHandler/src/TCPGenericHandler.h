#ifndef __TCPGenericHandler_h
#define __TCPGenericHandler_h

#include <stdint.h>
#include "application.h"

#define bufferSize 0xFF
#define terminatingByte 0x00

class TCPGenericHandler {

public:
  virtual bool connected() = 0;
  bool messageAvailable();
  bool tryReceiveMessage(String &out);
  void sendMessage(String data);

protected:
  TCPClient tcpClient;
  char encSndData[bufferSize];
  String encRecData;

  virtual void write(uint16_t index) = 0;

  void decrypt(String& in, String& out);
  void encrypt(String& in, char* out);

  String postProcess(String& message);
};

#endif
