#include <Arduino.h>
#include <WiFi.h>
#include "WiFiSetup/WiFiSetup.h"

void setup()
{
    Serial.begin(9600);

    String str = "";
    Serial.println("Start...");
    WiFiSetup::setup(str, str);
}

void loop()
{
    // put your main code here, to run repeatedly:
}