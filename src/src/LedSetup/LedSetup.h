#include <Arduino.h>

#include <WebServer.h>
#include <Preferences.h>
#include <esp_system.h>
#include <DNSServer.h>

#include "../WebServerExtensions/WebServerExtensions.h"

class LedSetup
{
public:
  static void setup();
  static void loadConfiguration();
  static int getAmountLeds();
  static bool isConfigurationValid();

private:
  static void logDebug(String message);

  static void handleRoot();
  static void handlePostConfiguration();

  static int amountLeds;
  static bool configurationValid;
  static bool doRestart;

  static const char *PREFERENCES_LEDSETUP;
  static const char *SETTING_LED_AMOUNT;

  static WebServer *webServer;
  static DNSServer *dnsServer;
};