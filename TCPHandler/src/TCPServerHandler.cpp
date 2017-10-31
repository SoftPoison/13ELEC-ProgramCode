#include <stdint.h>

#include "application.h"
#include "TCPServerHandler.h"

/** Constructor

  Initialises the TCPServerHandler

  @param port [0 .. 65535] the port to host the server on
*/
TCPServerHandler::TCPServerHandler(uint16_t port) {
  tcpServer = new TCPServer(port);
}

/** Sees if a client is connected

  Checks if a client is connected to the server

  @returns true if a client is connected, otherwise false
*/
bool TCPServerHandler::connected() {
  if (!tcpClient.connected()) {
    tcpClient = tcpServer->available();
    return tcpClient.connected();
  }

  return true;
}

void TCPServerHandler::write(uint16_t index) {
  if (index == 0) //Fixes weird issue where first byte is dropped
    (*tcpServer).write(0x02);

  (*tcpServer).write(encSndData[index]);
}
