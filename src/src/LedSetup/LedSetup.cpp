#include "LedSetup.h"

const char *LedSetup::PREFERENCES_LEDSETUP = "Led-Setup";
const char *LedSetup::SETTING_LED_AMOUNT = "ledAmount";
const char *LedSetup::SETTING_DATA_PIN = "dataPin";

WebServer *LedSetup::webServer = NULL;
DNSServer *LedSetup::dnsServer = NULL;
int LedSetup::amountLeds = 0;
bool LedSetup::doRestart = false;
bool LedSetup::configurationValid = false;

void LedSetup::loadConfiguration()
{
    Preferences preferences;
    preferences.begin(PREFERENCES_LEDSETUP, true);

    String ledAmount = preferences.getString(SETTING_LED_AMOUNT, String(""));

    configurationValid = false;
    if (ledAmount.length() != 0)
    {
        amountLeds = atoi(ledAmount.c_str());
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

    dnsServer->start(53, "*", WiFi.localIP());

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
        dnsServer->processNextRequest();
        delay(1);
    }

    ESP.restart();
}

int LedSetup::getAmountLeds()
{
    return amountLeds;
}

bool LedSetup::isConfigurationValid()
{
    return configurationValid;
}

void LedSetup::logDebug(String message)
{
#ifdef DEBUG
    Serial.print("LedSetup: ");
    Serial.println(message);
#endif
}

void LedSetup::handleRoot()
{
    logDebug("WebServer: Root called!");
    webServer->send(200, "text/html", pageLedSetupServerOk);
}

void LedSetup::handlePostConfiguration()
{
    logDebug("WebServer: Post configuration called!");

    int dataPin;
    int numLeds;

    if (!validateAndReadSettings(webServer, &dataPin, &numLeds))
    {
        webServer->send(400, "text/html", pageLedSetupServerInvalidSettings);
    }
    else
    {
        Preferences preferences;
        preferences.begin(PREFERENCES_LEDSETUP, false);

        preferences.putInt(SETTING_LED_AMOUNT, numLeds);
        preferences.putInt(SETTING_DATA_PIN, dataPin);

        preferences.end();

        webServer->send(200, "text/html", pageLedSetupServerOk);
        doRestart = true;
    }
}

bool LedSetup::validateAndReadSettings(WebServer *server, int *dataPin, int *numLeds)
{
    String dataPinString = webServer->arg("dataPin");
    String numLedsString = webServer->arg("numLeds");

    if (dataPinString.length() != 0 && numLedsString.length() != 0)
    {
        // Problem to solve: if toInt() fails, 0 is returned. But 0 is also a valid value for a pin. Thus we cannot check for error here in backend.
        *dataPin = dataPinString.toInt();
        *numLeds = numLedsString.toInt();

        if (*numLeds != 0)
        {
            return true;
        }
    }

    return false;
}
