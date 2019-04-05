#ifndef WiFiSetup_h_
#define WiFiSetup_h_

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "../web/PageWiFiSetupServerRoot.h"

class WiFiSetup
{
  public:
    static void setup(const WiFiServer &wifiServer, String ssid, String pwd);

  private:
    static void logDebug(String message);
    static void initializeWiFiConfigurationServer();

    static void handleNotFound();
    static void handleRoot();
    static void handlePostConfiguration();
    static void handleGetConfiguration();

    static WebServer *server;
};

#endif