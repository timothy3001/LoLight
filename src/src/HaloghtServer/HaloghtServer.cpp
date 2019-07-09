#include "HaloghtServer.h"

const char *HaloghtServer::FIELD_DEFAULT_COLOR = "defaultColor";
const char *HaloghtServer::FIELD_HOSTNAME_SUFFIX = "hostnameSuffix";

HaloghtServer::HaloghtServer(LedController *ledController)
{
    this->ledController = ledController;
    this->webServer = new AsyncWebServer(80);

    webServer->on("/", [&](AsyncWebServerRequest *request) -> void { this->handleRoot(request); });
    webServer->on("/settingsPage", [&](AsyncWebServerRequest *request) -> void { this->handleSettingsPage(request); });
    webServer->on("/state", HTTP_GET, [&](AsyncWebServerRequest *request) -> void { this->handleGetState(request); });
    webServer->on("/onOffState", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleOnOffSwitch(request); });
    webServer->on("/setSolidColor", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleSetSolidColor(request); });
    webServer->on("/setBrightness", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleSetBrightness(request); });
    webServer->on("/sendFire", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleFire(request); });
    webServer->on("/sendWater", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleWater(request); });
    webServer->on("/settings", HTTP_GET, [&](AsyncWebServerRequest *request) -> void { this->handleGetSettings(request); });
    webServer->on("/settings", HTTP_POST,
                  [](AsyncWebServerRequest *request) -> void { request->send(400, "text/plain", "No data!"); },
                  [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) -> void { request->send(400, "text/plain", "Wrong data!"); },
                  [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) -> void { this->handleUpdateSettings(request, data, len, index, total); });
    webServer->onFileUpload([&](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) -> void { this->handleUpdate(request, filename, index, data, len, final); });

    WebServerExtensions::registerBootstrap(*webServer);
    WebServerExtensions::registerNotFound(*webServer);

    webServer->begin();
}

HaloghtServer::~HaloghtServer()
{
    if (webServer)
        delete webServer;
}

void HaloghtServer::handleRoot(AsyncWebServerRequest *request)
{
    logDebug("Root called!");

    request->send(200, "text/html", pageHaloghtServerRoot);
}

void HaloghtServer::handleSettingsPage(AsyncWebServerRequest *request)
{
    logDebug("SettingsPage called!");

    request->send(200, "text/html", pageHaloghtServerSettings);
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

        HelperFunctions::extractColorFromString(request->arg("color"), &red, &green, &blue);

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

    // Flashing process
    // logDebug(String("Flashing ") + String(len) + String(" bytes..."));
    if (Update.write(data, len) != len)
    {
        Update.printError(Serial);
    }

    if (final)
    {
        if (Update.end(true))
        {
            // Flashing finished
            logDebug("Update Success!");
            logDebug("Rebooting...");

            AsyncWebServerResponse *response = request->beginResponse(200);
            response->addHeader("Connection", "close");
            request->send(response);

            delay(200);

            ESP.restart();
        }
        else
        {
            Update.printError(Serial);
        }
    }
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

void HaloghtServer::handleGetSettings(AsyncWebServerRequest *request)
{
    logDebug("GetSettings called!");

    Preferences preferencesLedSetup;
    Preferences preferencesWiFiSetup;

    preferencesLedSetup.begin(PREFERENCES_LEDSETUP, true);
    preferencesWiFiSetup.begin(PREFERENCES_WIFI, true);

    String defaultColor = preferencesLedSetup.getString(SETTING_DEFAULT_COLOR, "");
    String hostnameSuffix = preferencesWiFiSetup.getString(SETTING_HOSTNAME_SUFFIX, "");

    DynamicJsonDocument doc(1024);

    doc[FIELD_DEFAULT_COLOR] = defaultColor.c_str();
    doc[FIELD_HOSTNAME_SUFFIX] = hostnameSuffix.c_str();

    String json;
    serializeJson(doc, json);

    request->send(200, "application/json", json);
}

void HaloghtServer::handleUpdateSettings(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    logDebug("Update settings called!");

    if (len != total)
    {
        request->send(500, "text/plain", "Oops...!");
    }
    else
    {
        DynamicJsonDocument doc(len);
        if (deserializeJson(doc, data) != DeserializationError::Ok)
        {
            request->send(400, "text/plain", "Could not parse JSON!");
        }
        else if (!doc.containsKey(FIELD_DEFAULT_COLOR))
        {
            request->send(400, "text/plain", String("Missing parameter: ") + String(FIELD_DEFAULT_COLOR));
        }
        else if (!doc.containsKey(FIELD_HOSTNAME_SUFFIX))
        {
            request->send(400, "text/plain", String("Missing parameter: ") + String(FIELD_HOSTNAME_SUFFIX));
        }
        else
        {
            Preferences preferencesLedSetup;
            Preferences preferencesWiFiSetup;

            preferencesLedSetup.begin(PREFERENCES_LEDSETUP, false);
            preferencesWiFiSetup.begin(PREFERENCES_WIFI, false);

            String defaultColor = doc[FIELD_DEFAULT_COLOR];
            String hostnameSuffix = doc[FIELD_HOSTNAME_SUFFIX];

            preferencesLedSetup.putString(SETTING_DEFAULT_COLOR, defaultColor);
            preferencesWiFiSetup.putString(SETTING_HOSTNAME_SUFFIX, hostnameSuffix);

            preferencesLedSetup.end();
            preferencesWiFiSetup.end();

            request->send(200, "text/plain", "OK, restarting ESP...");
            delay(300);
            logDebug("Restarting ESP...");
            ESP.restart();
        }
    }
}

void HaloghtServer::logDebug(String message)
{
#ifdef DEBUG
    Serial.print("HaloghtServer: ");
    Serial.println(message);
#endif
}