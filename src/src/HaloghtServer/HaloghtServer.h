#ifndef HaloghtServer_h_
#define HaloghtServer_h_

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "../Constants.h"

#include "../LedController/LedController.h"
#include "../web/HaloghtServer/PageHaloghtServerRoot.h"
#include "../web/HaloghtServer/PageHaloghtServerSettings.h"
#include "../WebServerExtensions/WebServerExtensions.h"
#include "../Other/HelperFunctions.h"

class HaloghtServer
{
private:
    LedController *ledController;
    AsyncWebServer *webServer;

    static const char *FIELD_DEFAULT_COLOR;
    static const char *FIELD_HOSTNAME_SUFFIX;

    void handleSetBrightness(AsyncWebServerRequest *request);
    void handleRoot(AsyncWebServerRequest *request);
    void handleSettingsPage(AsyncWebServerRequest *request);
    void handleSetSolidColor(AsyncWebServerRequest *request);
    void handleFire(AsyncWebServerRequest *request);
    void handleWater(AsyncWebServerRequest *request);
    void handleOnOffSwitch(AsyncWebServerRequest *request);
    void handleGetState(AsyncWebServerRequest *request);
    void handleGetSettings(AsyncWebServerRequest *request);
    void handleUpdateSettings(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    void handleUpdate(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
    void handleUpdateUpload(AsyncWebServerRequest *request);
    void handleResetSettings(AsyncWebServerRequest *request);

    void logDebug(String message);

public:
    HaloghtServer(LedController *ledController);
    ~HaloghtServer();
};

#endif