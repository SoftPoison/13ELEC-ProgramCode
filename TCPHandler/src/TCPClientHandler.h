#ifndef __TCPClientHandler_h
#define __TCPClientHandler_h

#include <stdint.h>
#include "application.h"
#include "TCPGenericHandler.h"

class TCPClientHandler: public TCPGenericHandler {

public:
  TCPClientHandler(IPAddress address, uint16_t port);

  void connect();
  void disconnect();
  void setAddress(IPAddress address);
  void setPort(uint16_t port);
  bool connected(); //Overrides virtual member

protected:
  void write(uint16_t index);

private:
  IPAddress _address;
  uint16_t _port;

};

#endif
