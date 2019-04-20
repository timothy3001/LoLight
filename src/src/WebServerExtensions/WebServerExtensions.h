#ifndef WebServerExtensions_h_
#define WebServerExtensions_h_

#include <Arduino.h>
#include <WebServer.h>
#include "../web/bootstrap-min-css.h"

class WebServerExtensions
{
public:
  static void registerLargeFileEndpoint(String endPointName, String contentType, WebServer &server, const byte *file, int fileSize);
  static void registerBootstrap(WebServer &server);
  static void registerNotFound(WebServer &server);
};

#endif