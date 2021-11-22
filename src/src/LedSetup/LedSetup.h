#ifndef LedSetup_h_
#define LedSetup_h_

#include <Arduino.h>

#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <esp_system.h>

#include "../WebServerExtensions/WebServerExtensions.h"
#include "../web/LedSetup/PageLedSetupServerInvalidSettings.h"
#include "../web/LedSetup/PageLedSetupServerOk.h"
#include "../web/LedSetup/PageLedSetupServerRoot.h"
#include "../Constants.h"

class LedSetup
{
public:
  static void setup();
  static void loadConfiguration();
  static int getNumLeds();
  static int getDataPin();
  static int getLedType();
  static String &getDefaultColor();
  static bool isConfigurationValid();

private:
  static void logDebug(String message);

  static void handleRoot(AsyncWebServerRequest *request);
  static void handlePostConfiguration(AsyncWebServerRequest *request);
  static bool validateAndReadSettings(AsyncWebServerRequest *request, int *dataPin, int *numLeds, int *ledType);

  static int numLeds;
  static int dataPin;
  static int ledType;
  static bool configurationValid;
  static bool doRestart;
  static String defaultColor;

  static AsyncWebServer *webServer;
};

#endif