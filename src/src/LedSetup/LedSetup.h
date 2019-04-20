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
  static int getAmountLeds();

private:
  static void runLedSetupServer();
  static void logDebug(String message);

  static void handleRoot();
  static void handlePostConfiguration();

  static int amountLeds;
  static bool doRestart;

  static const char *PREFERENCES_LEDSETUP;
  static const char *SETTING_LED_AMOUNT;

  static WebServer *webServer;
  static DNSServer *dnsServer;
};