#ifndef LedSetup_h_
#define LedSetup_h_

#include <Arduino.h>

#include <WebServer.h>
#include <Preferences.h>
#include <esp_system.h>
#include <DNSServer.h>

#include "../WebServerExtensions/WebServerExtensions.h"
#include "../web/LedSetup/PageLedSetupServerInvalidSettings.h"
#include "../web/LedSetup/PageLedSetupServerOk.h"

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
  static bool validateAndReadSettings(WebServer *server, int *dataPin, int *numLeds);

  static int amountLeds;
  static bool configurationValid;
  static bool doRestart;

  static const char *PREFERENCES_LEDSETUP;
  static const char *SETTING_LED_AMOUNT;
  static const char *SETTING_DATA_PIN;

  static WebServer *webServer;
  static DNSServer *dnsServer;
};

#endif