#ifndef WiFiSetup_h_
#define WiFiSetup_h_

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <esp_system.h>
#include <DNSServer.h>
#include <ESPmDNS.h>

#include "../WebServerExtensions/WebServerExtensions.h"
#include "../web/WiFiSetup/PageWiFiSetupServerRoot.h"
#include "../web/WiFiSetup/PageWiFiSetupServerWrongSsid.h"
#include "../web/WiFiSetup/PageWiFiSetupServerOk.h"

class WiFiSetup
{
public:
  static void setup();
  static char *hostnamePrefix;

private:
  static const char *PREFERENCES_WIFI;
  static const char *SETTING_SSID;
  static const char *SETTING_PASSWORD;

  static bool doRestart;

  static void logDebug(String message);
  static void runWiFiConfigurationServer(String apName);
  static bool readWifiSettings(char *&ssid, char *&password);
  static void WiFiEventHandler(WiFiEvent_t event, system_event_info_t info);

  static void handleRoot();
  static void handlePostConfiguration();

  static String getHostname();

  static WebServer *server;
  static DNSServer *dnsServer;
};

#endif