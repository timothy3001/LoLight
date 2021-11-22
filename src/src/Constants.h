#ifndef Constants_h_
#define Constants_h_

#include <Arduino.h>

const char *const PREFERENCES_LEDSETUP = "Led-Setup";
const char *const SETTING_LED_AMOUNT = "ledAmount";
const char *const SETTING_DATA_PIN = "dataPin";
const char *const SETTING_DEFAULT_COLOR = "defaultColor";
const char *const SETTING_LED_TYPE = "ledType";
const int LED_TYPE_SK6812 = 0;
const int LED_TYPE_WS2815 = 1;

const char *const PREFERENCES_WIFI = "WiFi-Setup";
const char *const SETTING_SSID = "ssid";
const char *const SETTING_PASSWORD = "password";
const char *const SETTING_HOSTNAME_SUFFIX = "hostnameSuffix";

#endif