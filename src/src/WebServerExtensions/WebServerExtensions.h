#ifndef WebServerExtensions_h_
#define WebServerExtensions_h_

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "../web/Bootstrap/bootstrap-min-css.h"

class WebServerExtensions
{
public:
  static void registerLargeFileEndpoint(String endPointName, String contentType, AsyncWebServer &server, const byte *file, int fileSize);
  static void registerLargeFileChunkedEndpoint(String endPointName, String contentType, AsyncWebServer &server, const byte *file, int fileSize);
  static void registerBootstrap(AsyncWebServer &server);
  static void registerNotFound(AsyncWebServer &server);

private:
  static void logDebug(String msg);
};

#endif