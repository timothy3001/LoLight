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
    webServer->on("/sendStroboscope", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleStroboscope(request); });
    webServer->on("/settings", HTTP_GET, [&](AsyncWebServerRequest *request) -> void { this->handleGetSettings(request); });
    webServer->on(
        "/settings", HTTP_POST,
        [](AsyncWebServerRequest *request) -> void { request->send(400, "text/plain", "No data!"); },
        [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) -> void { request->send(400, "text/plain", "Wrong data!"); },
        [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) -> void { this->handleUpdateSettings(request, data, len, index, total); });
    webServer->on("/debugInfo", HTTP_GET, [&](AsyncWebServerRequest *request) -> void { this->handleDebugInfo(request); });
    webServer->onFileUpload([&](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) -> void { this->handleUpdate(request, filename, index, data, len, final); });
    webServer->on("/resetSettings", HTTP_POST, [&](AsyncWebServerRequest *request) -> void { this->handleResetSettings(request); });

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
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", (const byte *)pageHaloghtServerRoot, sizeof(pageHaloghtServerRoot) / sizeof(byte));
    request->send(response);
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

    ledController->setTwoColorBlendingAnimated(3500, false, true, 0, 32, 253, 103, 248, 255);
    request->send(200, "text/plain", "OK");
}

void HaloghtServer::handleStroboscope(AsyncWebServerRequest *request)
{
    logDebug("Stroboscope called!");

    ledController->setStroboscope(50, 255, 255, 255);
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

void HaloghtServer::handleDebugInfo(AsyncWebServerRequest *request)
{
    int millisPerDay = 1000 * 60 * 60 * 24;
    int millisPerHour = 1000 * 60 * 60;
    int millisPerMinute = 1000 * 60;
    int millisPerSecond = 1000;

    ulong now = millis();
    String upTime = "";
    int days = (int)(floor(now / (double)millisPerDay));
    int hours = (int)(floor((now % millisPerDay) / (double)millisPerHour));
    int minutes = (int)(floor((now % millisPerHour) / (double)millisPerMinute));
    int seconds = (int)(floor((now % millisPerMinute) / (double)millisPerSecond));

    String hourString = hours > 9 ? String(hours) : String("0") + String(hours);
    String minutesString = minutes > 9 ? String(minutes) : String("0") + String(minutes);
    String secondsString = seconds > 9 ? String(seconds) : String("0") + String(seconds);

    upTime += String("Uptime: ") + String(days) + String(" days, ") + hourString + String(":") + minutesString + String(":") + secondsString;

    request->send(200, "text/plain", upTime);
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

void HaloghtServer::handleResetSettings(AsyncWebServerRequest *request)
{
    logDebug("ResetSettings called!");

    Preferences preferencesLedSetup;
    Preferences preferencesWiFiSetup;

    preferencesLedSetup.begin(PREFERENCES_LEDSETUP, false);
    preferencesWiFiSetup.begin(PREFERENCES_WIFI, false);

    preferencesLedSetup.clear();
    preferencesWiFiSetup.clear();

    preferencesLedSetup.end();
    preferencesWiFiSetup.end();

    request->send(200, "text/plain", "OK, restarting ESP...");
    delay(300);
    logDebug("Settings reset, restarting ESP...");
    ESP.restart();
}

void HaloghtServer::logDebug(String message)
{
#ifdef DEBUG
    Serial.print("HaloghtServer: ");
    Serial.println(message);
#endif
}