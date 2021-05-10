#ifndef WiFiSetup_h_
#define WiFiSetup_h_

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <esp_system.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <rom/rtc.h>

#include "../WebServerExtensions/WebServerExtensions.h"
#include "../web/WiFiSetup/PageWiFiSetupServerRoot.h"
#include "../web/WiFiSetup/PageWiFiSetupServerWrongSsid.h"
#include "../web/WiFiSetup/PageWiFiSetupServerOk.h"
#include "../Constants.h"

class WiFiSetup
{
public:
  static void setup();
  static bool wasWorkaroundExecuted();
  static char *hostnamePrefix;

private:
  static String hostnameSuffix;
  static bool doRestart;
  static unsigned long rebootTimestamp;
  static const int timeBeforeReboot = 15 * 60 * 1000; // 15 minutes

  static void logDebug(String message);
  static void runWiFiConfigurationServer(String apName);
  static bool readWifiSettings(char *&ssid, char *&password);
  static void WiFiEventHandler(WiFiEvent_t event, system_event_info_t info);

  static void handleRoot(AsyncWebServerRequest *request);
  static void handlePostConfiguration(AsyncWebServerRequest *request);

  static String getShortMac();

  static AsyncWebServer *server;
  static DNSServer *dnsServer;
};

#endif