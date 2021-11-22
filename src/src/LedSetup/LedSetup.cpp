#include "LedSetup.h"

AsyncWebServer *LedSetup::webServer = NULL;
int LedSetup::numLeds = -1;
int LedSetup::dataPin = -1;
int LedSetup::ledType = -1;
bool LedSetup::doRestart = false;
bool LedSetup::configurationValid = false;
String LedSetup::defaultColor = "";

void LedSetup::loadConfiguration()
{
    Preferences preferences;
    preferences.begin(PREFERENCES_LEDSETUP, false);

    configurationValid = false;

    numLeds = preferences.getInt(SETTING_LED_AMOUNT, -1);
    dataPin = preferences.getInt(SETTING_DATA_PIN, -1);
    ledType = preferences.getInt(SETTING_LED_TYPE, -1);
    defaultColor = preferences.getString(SETTING_DEFAULT_COLOR, "");

    if (defaultColor.length() == 0)
    {
        defaultColor = "#DCAA5A";
        preferences.putString(SETTING_DEFAULT_COLOR, defaultColor);
    }

    if (numLeds > 0 && dataPin >= 0)
    {
        configurationValid = true;
    }

    preferences.end();
}

void LedSetup::setup()
{
    logDebug("Starting WebServer for LED setup...");
    webServer = new AsyncWebServer(80);

    webServer->on("/", handleRoot);
    webServer->on("/config", HTTP_POST, handlePostConfiguration);
    WebServerExtensions::registerBootstrap(*webServer);

    webServer->begin();

    while (!doRestart)
    {
        delay(1);
    }

    logDebug("ESP about to restart...");
    unsigned long tsWaitForRestart = millis();
    while (tsWaitForRestart + 10000 > millis())
    {
        delay(1);
    }

    ESP.restart();
}

int LedSetup::getNumLeds()
{
    return numLeds;
}

int LedSetup::getDataPin()
{
    return dataPin;
}

int LedSetup::getLedType()
{
    return ledType;
}

String &LedSetup::getDefaultColor()
{
    return defaultColor;
}

bool LedSetup::isConfigurationValid()
{
    return configurationValid;
}

void LedSetup::handleRoot(AsyncWebServerRequest *request)
{
    logDebug("WebServer: Root called!");
    request->send(200, "text/html", pageLedSetupServerRoot);
}

void LedSetup::handlePostConfiguration(AsyncWebServerRequest *request)
{
    logDebug("WebServer: Post configuration called!");

    if (!validateAndReadSettings(request, &dataPin, &numLeds, &ledType))
    {
        logDebug("Invalid parameters!");
        request->send(400, "text/html", pageLedSetupServerInvalidSettings);
    }
    else
    {
        logDebug(String("Valid configuration submitted: dataPin: '") + String(dataPin) + String("' numLeds: '") + String(numLeds) + String("'"));
        Preferences preferences;
        preferences.begin(PREFERENCES_LEDSETUP, false);

        preferences.putInt(SETTING_LED_AMOUNT, numLeds);
        preferences.putInt(SETTING_DATA_PIN, dataPin);
        preferences.putInt(SETTING_LED_TYPE, ledType);

        preferences.end();

        request->send(200, "text/html", pageLedSetupServerOk);
        doRestart = true;
    }
}

bool LedSetup::validateAndReadSettings(AsyncWebServerRequest *request, int *postDataPin, int *postNumLeds, int *postLedType)
{
    String dataPinString = request->hasArg("dataPin") ? request->arg("dataPin") : String("");
    String numLedsString = request->hasArg("numLeds") ? request->arg("numLeds") : String("");
    String ledTypeString = request->hasArg("ledType") ? request->arg("ledType") : String("");

    if (dataPinString.length() != 0 && numLedsString.length() != 0 && ledTypeString.length())
    {
        // Problem to solve: if toInt() fails, 0 is returned. But 0 is also a valid value for a pin. Thus we cannot check for error here in backend.
        *postDataPin = dataPinString.toInt();
        *postNumLeds = numLedsString.toInt();
        *postLedType = ledTypeString.toInt();

        if (*postNumLeds != 0)
        {
            return true;
        }
    }

    return false;
}

void LedSetup::logDebug(String message)
{
#ifdef DEBUG
    Serial.print("LedSetup: ");
    Serial.println(message);
#endif
}