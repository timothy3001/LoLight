#include "LedSetup.h"

const char *LedSetup::PREFERENCES_LEDSETUP = "Led-Setup";
const char *LedSetup::SETTING_LED_AMOUNT = "ledAmount";
WebServer *LedSetup::webServer = NULL;
DNSServer *LedSetup::dnsServer = NULL;
int LedSetup::amountLeds = 0;
bool LedSetup::doRestart = false;

void LedSetup::setup()
{
    Preferences preferences;

    preferences.begin(PREFERENCES_LEDSETUP, true);

    String ledAmount = preferences.getString(SETTING_LED_AMOUNT, String(""));
    if (ledAmount.length() != 0)
    {
        amountLeds = atoi(ledAmount.c_str());
    }
    else
    {
        runLedSetupServer();
    }
}

void LedSetup::runLedSetupServer()
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
}

void LedSetup::handlePostConfiguration()
{
    logDebug("WebServer: Post configuration called!");
}
