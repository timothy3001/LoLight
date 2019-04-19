#include <Arduino.h>
#include <WiFi.h>
#include "WiFiSetup/WiFiSetup.h"

void setup()
{
    Serial.begin(9600);

    Serial.println("Starting WiFi setup...");
    WiFiSetup::hostnamePrefix = "Haloght";

    // In case WiFi needs to be setup this is a blocking call and the ESP will be rebooted. After this call, WiFi works fine.
    WiFiSetup::setup();
}

void loop()
{
    // put your main code here, to run repeatedly:
}