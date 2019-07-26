#ifndef WebServerExtensions_h_
#define WebServerExtensions_h_

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "../web/Bootstrap/bootstrap-min-css.h"
#include "../web/Bootstrap/bootstrap-min-js.h"
#include "../web/Bootstrap/jquery-3-3-1-slim-min-js.h"

class WebServerExtensions
{
public:
  static void registerLargeFileEndpoint(String endPointName, String contentType, AsyncWebServer &server, const byte *file, int fileSize);
  static void registerBootstrap(AsyncWebServer &server);
  static void registerNotFound(AsyncWebServer &server);

private:
  static void logDebug(String msg);
};

#endif