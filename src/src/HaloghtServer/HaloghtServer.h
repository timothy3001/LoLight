#ifndef HaloghtServer_h_
#define HaloghtServer_h_

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>

#include "../LedController/LedController.h"
#include "../web/HaloghtServer/PageHaloghtServerRoot.h"
#include "../WebServerExtensions/WebServerExtensions.h"

class HaloghtServer
{
private:
    LedController *ledController;
    AsyncWebServer *webServer;

    void setupOTA();

    void handleSetBrightness(AsyncWebServerRequest *request);
    void handleRoot(AsyncWebServerRequest *request);
    void handleSetSolidColor(AsyncWebServerRequest *request);
    void handleFire(AsyncWebServerRequest *request);
    void handleWater(AsyncWebServerRequest *request);
    void handleOnOffSwitch(AsyncWebServerRequest *request);
    void handleGetState(AsyncWebServerRequest *request);

    void handleUpdate(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
    void handleUpdateUpload(AsyncWebServerRequest *request);

    void extractColorFromString(String str, uint8_t *r, uint8_t *g, uint8_t *b);
    void logDebug(String message);

    

public:
    HaloghtServer(LedController *ledController);
    ~HaloghtServer();
    void handle();
};

#endif