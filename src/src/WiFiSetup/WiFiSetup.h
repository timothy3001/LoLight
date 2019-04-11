#ifndef WiFiSetup_h_
#define WiFiSetup_h_

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <esp_system.h>

#include "../WebServerExtensions/WebServerExtensions.h"
#include "../web/WiFiSetup/PageWiFiSetupServerRoot.h"
#include "../web/WiFiSetup/PageWiFiSetupServerWrongSsid.h"
#include "../web/WiFiSetup/PageWiFiSetupServerOk.h"
#include "../web/bootstrap-min-css.h"

class WiFiSetup
{
public:
  static void setup();

private:
  static const char *PREFERENCES_WIFI;
  static const char *SETTING_SSID;
  static const char *SETTING_PASSWORD;

  static void logDebug(String message);
  static void runWiFiConfigurationServer();
  static bool readWifiSettings(char *&ssid, char *&password);

  static void handleNotFound();
  static void handleRoot();
  static void handlePostConfiguration();

  static WebServer *server;
};

#endif