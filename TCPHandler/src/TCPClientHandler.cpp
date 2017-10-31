#include <stdint.h>

#include "application.h"
#include "TCPClientHandler.h"

/** Constructor

  Initialises the TCPClientHandler
  Passing `nullptr` instead of an IPAddress is a valid way of initialising the
  class without passing a dummy IPAddress

  @param address the IPAddress to connect to

  @param port [0 .. 65535] the port the server is running on
*/
TCPClientHandler::TCPClientHandler(IPAddress address, uint16_t port) {
  _address = address;
  _port = port;
}

/** Sets internal IPAddress

  Sets the internal IPAddress. Running `TCPClientHandler::connected()` following
  this is probably a good idea.

  @param address the IPAddress to connect to
*/
void TCPClientHandler::setAddress(IPAddress address) {
  _address = address;
}

/** Sets internal port

  Sets the internal port. Running `TCPClientHandler::connected()` following
  this is probably a good idea.

  @param port [0 .. 65535] the port the server is running on
*/
void TCPClientHandler::setPort(uint16_t port) {
  _port = port;
}

/** Connects to server

  Attempts to connect to the server. It may be a better idea to call
  `TCPClientHandler::connected()` as it does the same thing and checks for
  connectivity too
*/
void TCPClientHandler::connect() {
  tcpClient.connect(_address, _port);
}

/** Disconnects from server

  Disconnects the client from the server safely
*/
void TCPClientHandler::disconnect() {
  tcpClient.stop();
}

/** Sees if client is connected

  Checks if client is connected to the server. If it isn't yet connected, it
  will then try and attempt to connect

  @returns true if device is connected, otherwise false
*/
bool TCPClientHandler::connected() {
  return tcpClient.connected();
}

void TCPClientHandler::write(uint16_t index) {
  tcpClient.write(encSndData[index]);
}
