#include "LedSetup.h"

const char *LedSetup::PREFERENCES_LEDSETUP = "Led-Setup";
const char *LedSetup::SETTING_LED_AMOUNT = "ledAmount";
const char *LedSetup::SETTING_DATA_PIN = "dataPin";

WebServer *LedSetup::webServer = NULL;
int LedSetup::numLeds = -1;
int LedSetup::dataPin = -1;
bool LedSetup::doRestart = false;
bool LedSetup::configurationValid = false;

void LedSetup::loadConfiguration()
{
    Preferences preferences;
    preferences.begin(PREFERENCES_LEDSETUP, true);

    configurationValid = false;

    numLeds = preferences.getInt(SETTING_LED_AMOUNT, -1);
    dataPin = preferences.getInt(SETTING_DATA_PIN, -1);

    if (numLeds > 0 && dataPin >= 0)
    {
        configurationValid = true;
    }
}

void LedSetup::setup()
{
    logDebug("Starting WebServer for LED setup...");
    webServer = new WebServer(80);

    webServer->on("/", handleRoot);
    webServer->on("/config", HTTP_POST, handlePostConfiguration);
    WebServerExtensions::registerBootstrap(*webServer);

    webServer->begin();

    while (!doRestart)
    {
        webServer->handleClient();
        delay(1);
    }

    logDebug("ESP about to restart...");
    unsigned long tsWaitForRestart = millis();
    while (tsWaitForRestart + 10000 > millis())
    {
        webServer->handleClient();
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

bool LedSetup::isConfigurationValid()
{
    return configurationValid;
}

void LedSetup::handleRoot()
{
    logDebug("WebServer: Root called!");
    webServer->send(200, "text/html", pageLedSetupServerRoot);
}

void LedSetup::handlePostConfiguration()
{
    logDebug("WebServer: Post configuration called!");

    if (!validateAndReadSettings(webServer, &dataPin, &numLeds))
    {
        logDebug("Invalid parameters!");
        webServer->send(400, "text/html", pageLedSetupServerInvalidSettings);
    }
    else
    {
        logDebug(String("Valid configuration submitted: dataPin: '") + String(dataPin) + String("' numLeds: '") + String(numLeds) + String("'"));
        Preferences preferences;
        preferences.begin(PREFERENCES_LEDSETUP, false);

        preferences.putInt(SETTING_LED_AMOUNT, numLeds);
        preferences.putInt(SETTING_DATA_PIN, dataPin);

        preferences.end();

        webServer->send(200, "text/html", pageLedSetupServerOk);
        doRestart = true;
    }
}

bool LedSetup::validateAndReadSettings(WebServer *server, int *postDataPin, int *postNumLeds)
{
    String dataPinString = webServer->arg("dataPin");
    String numLedsString = webServer->arg("numLeds");

    if (dataPinString.length() != 0 && numLedsString.length() != 0)
    {
        // Problem to solve: if toInt() fails, 0 is returned. But 0 is also a valid value for a pin. Thus we cannot check for error here in backend.
        *postDataPin = dataPinString.toInt();
        *postNumLeds = numLedsString.toInt();

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