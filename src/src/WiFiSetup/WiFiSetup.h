#ifndef WiFiSetup_h_
#define WiFiSetup_h_

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <esp_system.h>

#include "../WebServerExtensions/WebServerExtensions.h"
#include "../web/PageWiFiSetupServerRoot.h"
#include "../web/bootstrap-min-css.h"

class WiFiSetup
{
public:
  static void setup(const String ssid, const String pwd);

private:
  static void logDebug(String message);
  static void runWiFiConfigurationServer();

  static void handleNotFound();
  static void handleRoot();
  static void handlePostConfiguration();
  static void handleGetConfiguration();

  static WebServer *server;
};

#endif