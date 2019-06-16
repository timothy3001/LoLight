#ifndef HaloghtServer_h_
#define HaloghtServer_h_

#include <Arduino.h>
#include <WebServer.h>
#include <Update.h>

#include "../LedController/LedController.h"
#include "../web/HaloghtServer/PageHaloghtServerRoot.h"
#include "../WebServerExtensions/WebServerExtensions.h"

class HaloghtServer
{
private:
    LedController *ledController;
    WebServer *webServer;

    void handleSetBrightness();
    void handleRoot();
    void handleSetSolidColor();
    void handleFire();
    void handleWater();
    void handleOnOffSwitch();
    void handleGetState();

    void handleUpdate();
    void handleUpdateUpload();

    void extractColorFromString(String str, uint8_t *r, uint8_t *g, uint8_t *b);
    void logDebug(String message);

public:
    HaloghtServer(LedController *ledController);
    ~HaloghtServer();
    void handleClient();
    void setupWebServer();
};

#endif