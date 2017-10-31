#ifndef __TCPServerHandler_h
#define __TCPServerHandler_h

#include <stdint.h>
#include "application.h"
#include "TCPGenericHandler.h"

class TCPServerHandler: public TCPGenericHandler {

public:
  TCPServerHandler(uint16_t port);

  bool connected(); //Overrides virtual member

protected:
  void write(uint16_t index); //Overrides virtual member

private:
  TCPServer* tcpServer;

};

#endif
