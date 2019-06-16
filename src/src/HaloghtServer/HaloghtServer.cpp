#include "HaloghtServer.h"

HaloghtServer::HaloghtServer(LedController *ledController)
{
    this->ledController = ledController;
    this->webServer = new WebServer(80);

    webServer->on("/", [&]() -> void { this->handleRoot(); });
    webServer->on("/state", HTTP_GET, [&]() -> void { this->handleGetState(); });
    webServer->on("/onOffState", HTTP_POST, [&]() -> void { this->handleOnOffSwitch(); });
    webServer->on("/setSolidColor", HTTP_POST, [&]() -> void { this->handleSetSolidColor(); });
    webServer->on("/setBrightness", HTTP_POST, [&]() -> void { this->handleSetBrightness(); });
    webServer->on("/sendFire", HTTP_POST, [&]() -> void { this->handleFire(); });
    webServer->on("/sendWater", HTTP_POST, [&]() -> void { this->handleWater(); });
    webServer->on("/update", HTTP_POST, [&]() -> void { this->handleUpdate(); }, [&]() -> void { this->handleUpdateUpload(); });
    WebServerExtensions::registerBootstrap(*webServer);
    WebServerExtensions::registerNotFound(*webServer);

    webServer->begin();
}

HaloghtServer::~HaloghtServer()
{
    if (webServer)
        delete webServer;
}

void HaloghtServer::handleClient()
{
    webServer->handleClient();
}

void HaloghtServer::handleRoot()
{
    logDebug("Root called!");

    webServer->send(200, "text/html", pageHaloghtServerRoot);
}

void HaloghtServer::handleSetBrightness()
{
    logDebug("SetBrightness called!");

    String brightnessString = webServer->arg("brightness");
    if (!brightnessString || brightnessString.length() == 0)
    {
        webServer->send(400, "text/plain", "No brightness or invalid brightness submitted!");
    }
    else
    {
        ledController->setBrightness(brightnessString.toFloat());
        webServer->send(200, "text/plain", "OK");
    }
}

void HaloghtServer::handleOnOffSwitch()
{
    logDebug("OnOff called!");

    String onOffStateString = webServer->arg("on");
    if (!onOffStateString || onOffStateString.length() == 0)
    {
        webServer->send(400, "text/plain", "No state or invalid state sent!");
    }
    else
    {
        onOffStateString.toLowerCase();

        if (onOffStateString.equals("true"))
            ledController->setOnOff(true);
        else
            ledController->setOnOff(false);

        webServer->send(200, "text/plain", "OK");
    }
}

void HaloghtServer::handleSetSolidColor()
{
    logDebug("SetSolidColor called!");

    String colorString = webServer->arg("color");
    if (colorString && colorString.length() == 7)
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;

        extractColorFromString(colorString, &red, &green, &blue);

        ledController->setSolidColor(red, green, blue);
        webServer->send(200, "text/plain", "OK");
    }
    else
    {
        webServer->send(400, "text/plain", "Wrong Format!");
    }
}

void HaloghtServer::handleFire()
{
    logDebug("SendFire called!");

    ledController->setTwoColorBlendingAnimated(1700, true, false, 255, 23, 23, 255, 238, 23);
    webServer->send(200, "text/plain", "OK");
}

void HaloghtServer::handleWater()
{
    logDebug("SendWater called!");

    ledController->setTwoColorBlendingAnimated(5000, false, true, 0, 32, 253, 85, 242, 255);
    webServer->send(200, "text/plain", "OK");
}

void HaloghtServer::handleUpdate()
{
    logDebug("Handling update...");

    webServer->sendHeader("Connection", "close");
    webServer->send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
}

void HaloghtServer::handleUpdateUpload()
{
    HTTPUpload &upload = webServer->upload();
    if (upload.status == UPLOAD_FILE_START)
    {
        logDebug(String("Update: ") + String(upload.filename.c_str()));
        if (!Update.begin(UPDATE_SIZE_UNKNOWN))
        {
            // Error while flashing new software
            Update.printError(Serial);
        }
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
        // Flashing process
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
        {
            Update.printError(Serial);
        }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
        if (Update.end(true))
        {
            // Flashing finished
            logDebug("Update Success: " + String(upload.totalSize) + String(" bytes updated!"));
            logDebug("Rebooting...");
        }
        else
        {
            Update.printError(Serial);
        }
    }
}

void HaloghtServer::handleGetState()
{
    logDebug("GetState called!");

    String stateString = String("{\n");
    stateString += String("\t\"turnedOn\":") + String(ledController->isTurnedOn()) + String(",\n");
    stateString += String("\t\"brightness\":") + String(ledController->getBrightness()) + String("\n");
    stateString += String("}");

    webServer->send(200, "application/json", stateString);
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

void HaloghtServer::logDebug(String message)
{
#ifdef DEBUG
    Serial.print("HaloghtServer: ");
    Serial.println(message);
#endif
}