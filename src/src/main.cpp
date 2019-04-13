#include <Arduino.h>
#include <WiFi.h>
#include "WiFiSetup/WiFiSetup.h"

void setup()
{
    Serial.begin(9600);

    Serial.println("Starting WiFi setup...");
    WiFiSetup::hostnamePrefix = "Haloght";
    WiFiSetup::setup();
}

void loop()
{
    // put your main code here, to run repeatedly:
}