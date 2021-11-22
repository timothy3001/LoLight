#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <HTTPClient.h>

#include "WiFiSetup/WiFiSetup.h"
#include "LedSetup/LedSetup.h"
#include "LedController/LedController.h"
#include "HaloghtServer/HaloghtServer.h"

#include <rom/rtc.h>

LedController *ledController;
HaloghtServer *haloghtServer;

HTTPClient http;

// Used for debugging purposes. Keeping this in code so I don't have to look it up every time.
// void printRebootReasons()
// {
//     String reason = String("Current: ") + String("CPU 0: ") + String(rtc_get_reset_reason(0)) + String(" CPU 1: ") + String(rtc_get_reset_reason(1));

//     Serial.println(reason);
// }

void setup()
{
    Serial.begin(9600);

    // printRebootReasons();

    Serial.println("Reading LED config...");
    LedSetup::loadConfiguration();

    if (LedSetup::isConfigurationValid() && !WiFiSetup::wasWorkaroundExecuted())
    {
        // Turn LEDs on to desired state (only if configuration is valid and the ESP wasn't reset by software.
        // Currently a workaround to prevent turning on after a ESP reset due to WiFi)
        Serial.println("Valid config found, turning LEDs on to desired state...");
        ledController = new LedController(LedSetup::getDataPin(), LedSetup::getNumLeds(), LedSetup::getLedType(), LedSetup::getDefaultColor());

        ledController->setDefault();
        ledController->handle();
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
    if (!ledController)
        ledController = new LedController(LedSetup::getDataPin(), LedSetup::getNumLeds(), LedSetup::getLedType(), LedSetup::getDefaultColor());

    haloghtServer = new HaloghtServer(ledController);
}

void loop()
{
    ledController->handle();
}