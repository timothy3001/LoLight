#ifndef WebServerExtensions_h_
#define WebServerExtensions_h_

#include <Arduino.h>
#include <WebServer.h>

class WebServerExtensions
{
public:
  static void registerLargeFileEndpoint(String endPointName, String contentType, WebServer &server, const byte *file, int fileSize);
};

#endif