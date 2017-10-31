#include "application.h"
#include "TCPGenericHandler.h"

/** Checks for message

  Checks to see if there is a message available to be read

  @returns true if there is a message, otherwise false
*/
bool TCPGenericHandler::messageAvailable() {
  return tcpClient.available() > 0;
}

/** Receives message

  Attempts to receive message from client/server in a relatively safe fashion

  @param &out the String object to write to

  @returns true if a valid message was successfully received, otherwise false
*/
bool TCPGenericHandler::tryReceiveMessage(String &out) {
  if (!messageAvailable())
    return false;

  encRecData = tcpClient.readStringUntil((char) terminatingByte);

  decrypt(encRecData, out);

  return true;
}

/** Sends a message

  Sends a given message to the connected client/server

  @param data the string to send
*/
void TCPGenericHandler::sendMessage(String data) {
  data.trim();
  data += (char) terminatingByte; //Make sure that the string ends correctly
  encrypt(data, encSndData);

  bool terminateMessage = false;

  for (uint16_t i = 0; i < bufferSize; i++) {
    if (terminateMessage)
      break;

    if (encSndData[i] == terminatingByte)
      terminateMessage = true;

    write(i);
  }
}

void TCPGenericHandler::decrypt(String& in, String& out) { //Will be populated later
  out = in;
  out = postProcess(out);
  out.trim();
}

void TCPGenericHandler::encrypt(String& in, char* out) { //Will be populated later
  in.toCharArray(out, bufferSize);
}

String TCPGenericHandler::postProcess(String& message) {
  if ((int8_t) message.charAt(0) == 0x02)
    return message.substring(1);
  else
    return message;
}
