#include "HaloghtServer.h"

HaloghtServer::HaloghtServer(LedController *ledController)
{
    this->ledController = ledController;
    this->webServer = new AsyncWebServer(80);

    webServer->on("/", [&](AsyncWebServerRequest *request) -> void { this->handleRoot(request); });
    webServer->on("/state", HTTP_GET, [&](AsyncWebServerRequest *request) -> void { this->handleGetState(request); });
    webServer->on("/onOffState", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleOnOffSwitch(request); });
    webServer->on("/setSolidColor", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleSetSolidColor(request); });
    webServer->on("/setBrightness", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleSetBrightness(request); });
    webServer->on("/sendFire", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleFire(request); });
    webServer->on("/sendWater", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleWater(request); });
    webServer->onFileUpload([&](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) -> void { this->handleUpdate(request, filename, index, data, len, final); });
    WebServerExtensions::registerBootstrap(*webServer);
    WebServerExtensions::registerNotFound(*webServer);

    webServer->begin();

    setupOTA();
}

HaloghtServer::~HaloghtServer()
{
    if (webServer)
        delete webServer;
}

void HaloghtServer::setupOTA()
{
    ArduinoOTA.onStart([&]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
        {
            type = "sketch";
        }
        else
        { // U_FS
            type = "filesystem";
        }

        logDebug(String("Start updating ") + String(type));
    });
    ArduinoOTA.onEnd([&]() {
        logDebug("\nEnd");
    });
    ArduinoOTA.onProgress([&](unsigned int progress, unsigned int total) {
        logDebug(String("Progress: ") + String(round(progress / (total / (float)100))) + String("%"));
    });
    ArduinoOTA.onError([&](ota_error_t error) {
        logDebug(String("Error[") + String(error) + String("]: "));
        if (error == OTA_AUTH_ERROR)
        {
            logDebug("Auth Failed");
        }
        else if (error == OTA_BEGIN_ERROR)
        {
            logDebug("Begin Failed");
        }
        else if (error == OTA_CONNECT_ERROR)
        {
            logDebug("Connect Failed");
        }
        else if (error == OTA_RECEIVE_ERROR)
        {
            logDebug("Receive Failed");
        }
        else if (error == OTA_END_ERROR)
        {
            logDebug("End Failed");
        }
    });
    ArduinoOTA.begin();
}

void HaloghtServer::handleRoot(AsyncWebServerRequest *request)
{
    logDebug("Root called!");

    request->send(200, "text/html", pageHaloghtServerRoot);
}

void HaloghtServer::handleSetBrightness(AsyncWebServerRequest *request)
{
    logDebug("SetBrightness called!");

    if (!request->hasArg("brightness") || request->arg("brightness").length() == 0)
    {
        request->send(400, "text/plain", "No brightness or invalid brightness submitted!");
    }
    else
    {
        String brightnessString = request->arg("brightness");
        ledController->setBrightness(brightnessString.toFloat());
        request->send(200, "text/plain", "OK");
    }
}

void HaloghtServer::handleOnOffSwitch(AsyncWebServerRequest *request)
{
    logDebug("OnOff called!");

    if (!request->hasArg("on") || request->arg("on").length() == 0)
    {
        request->send(400, "text/plain", "No state or invalid state sent!");
    }
    else
    {
        String onOffStateString = request->arg("on");

        onOffStateString.toLowerCase();

        if (onOffStateString.equals("true"))
            ledController->setOnOff(true);
        else
            ledController->setOnOff(false);

        request->send(200, "text/plain", "OK");
    }
}

void HaloghtServer::handleSetSolidColor(AsyncWebServerRequest *request)
{
    logDebug("SetSolidColor called!");

    if (request->hasArg("color") && request->arg("color").length() == 7)
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;

        extractColorFromString(request->arg("color"), &red, &green, &blue);

        ledController->setSolidColor(red, green, blue);
        request->send(200, "text/plain", "OK");
    }
    else
    {
        request->send(400, "text/plain", "Wrong Format!");
    }
}

void HaloghtServer::handleFire(AsyncWebServerRequest *request)
{
    logDebug("SendFire called!");

    ledController->setTwoColorBlendingAnimated(1700, true, false, 255, 23, 23, 255, 238, 23);
    request->send(200, "text/plain", "OK");
}

void HaloghtServer::handleWater(AsyncWebServerRequest *request)
{
    logDebug("SendWater called!");

    ledController->setTwoColorBlendingAnimated(5000, false, true, 0, 32, 253, 85, 242, 255);
    request->send(200, "text/plain", "OK");
}

void HaloghtServer::handleUpdate(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        logDebug(String("Update: ") + String(filename.c_str()));
        if (!Update.begin(UPDATE_SIZE_UNKNOWN))
        {
            // Error while flashing new software
            Update.printError(Serial);
        }
    }
    for (size_t i = 0; i < len; i++)
    {
        // Flashing process
        if (Update.write(data, len) != len)
        {
            Update.printError(Serial);
        }
    }
    if (final)
    {
        if (Update.end(true))
        {
            // Flashing finished
            logDebug("Update Success!");
            logDebug("Rebooting...");
        }
        else
        {
            Update.printError(Serial);
        }
    }

    AsyncWebServerResponse *response = request->beginResponse(200); //Sends 404 File Not Found
    response->addHeader("Connection", "close");
    request->send(response);

    ESP.restart();
}

void HaloghtServer::handleGetState(AsyncWebServerRequest *request)
{
    logDebug("GetState called!");

    String stateString = String("{\n");
    stateString += String("\t\"turnedOn\":") + String(ledController->isTurnedOn()) + String(",\n");
    stateString += String("\t\"brightness\":") + String(ledController->getBrightness()) + String("\n");
    stateString += String("}");

    request->send(200, "application/json", stateString);
}

void HaloghtServer::extractColorFromString(String str, uint8_t *r, uint8_t *g, uint8_t *b)
{
    char color[8];
    str.toCharArray(color, 8);

    char redPart[3];
    char greenPart[3];
    char bluePart[3];

    strncpy(redPart, color + 1, 2);
    redPart[2] = 0;
    strncpy(greenPart, color + 3, 2);
    greenPart[2] = 0;
    strncpy(bluePart, color + 5, 2);
    bluePart[2] = 0;

    *r = strtol(redPart, NULL, 16);
    *g = strtol(greenPart, NULL, 16);
    *b = strtol(bluePart, NULL, 16);
}

void handle()
{
    ArduinoOTA.handle();
}

void HaloghtServer::logDebug(String message)
{
#ifdef DEBUG
    Serial.print("HaloghtServer: ");
    Serial.println(message);
#endif
}