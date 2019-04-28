#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

#include "WiFiSetup/WiFiSetup.h"
#include "LedSetup/LedSetup.h"
#include "LedController/LedController.h"

LedController *ledController;

void resetPreferences()
{
    Preferences preferencesLedSetup;
    Preferences preferencesWiFiSetup;
    preferencesLedSetup.begin("Led-Setup", false);
    preferencesWiFiSetup.begin("WiFi-Setup", false);
    preferencesLedSetup.clear();
    preferencesWiFiSetup.clear();
    preferencesLedSetup.end();
    preferencesWiFiSetup.end();
}

void setup()
{
    // Reset preferences
    //resetPreferences();

    Serial.begin(9600);

    Serial.println("Reading LED config...");
    LedSetup::loadConfiguration();

    if (LedSetup::isConfigurationValid())
    {
        // Turn LEDs on to desired state
        Serial.println("Valid config found, turning LEDs on to desired state...");
        ledController = new LedController(LedSetup::getDataPin(), LedSetup::getNumLeds());

        ledController->setSolidColor(127, 127, 127);
    }

    Serial.println("Starting WiFi setup...");
    WiFiSetup::hostnamePrefix = "Haloght";

    // In case WiFi needs to be setup this is a blocking call and the ESP will be rebooted. After this call, WiFi works fine.
    WiFiSetup::setup();

    // LED settings will be read from preferences. If it is not configured yet a web server for configuration will be started and the ESP reboots afterwards.
    if (!LedSetup::isConfigurationValid())
    {
        Serial.println("LED config is not valid thus starting LED setup...");
        LedSetup::setup();
    }

    Serial.println("WiFi and LEDs setup successful! Starting regular operation...");
}

void loop()
{
    // put your main code here, to run repeatedly:
}