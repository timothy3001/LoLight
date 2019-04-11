#include "WiFiSetup.h"

WebServer *WiFiSetup::server = NULL;
const char *WiFiSetup::PREFERENCES_WIFI = "WiFi-Setup";
const char *WiFiSetup::SETTING_SSID = "ssid";
const char *WiFiSetup::SETTING_PASSWORD = "password";

bool WiFiSetup::readWifiSettings(char *&ssid, char *&password)
{
    Preferences preferences;

    preferences.begin(PREFERENCES_WIFI, true);

    String ssidString = preferences.getString(SETTING_SSID, String(""));

    bool result = false;
    if (ssidString.length() != 0)
    {
        String passwordString = preferences.getString(SETTING_PASSWORD, String(""));

        ssid = new char[ssidString.length() + 1];
        password = new char[passwordString.length() + 1];

        strcpy(ssid, ssidString.c_str());
        strcpy(password, passwordString.c_str());

        result = true;
    }

    preferences.end();

    return result;
}

void WiFiSetup::setup()
{
    char *ssid = 0;
    char *password = 0;

    bool result = readWifiSettings(ssid, password);

    if (result)
    {
        // Try to connect to the access point
        logDebug(String("Connecting to access point with SSID '") + String(ssid) + String("' and password '") + String(password) + String("'..."));

        WiFi.mode(WIFI_AP);
        WiFi.begin(ssid, password);

        unsigned long startTimeConnecting = millis();
        while (WiFi.status() != WL_CONNECTED && millis() < startTimeConnecting + 30000)
        {
            delay(200);
        }

        if (WiFi.status() != WL_CONNECTED)
        {
            logDebug("Could not connect to configured access point, creating access point...");
            runWiFiConfigurationServer();
        }
        else
        {
            logDebug("Successfully connected to WiFi!");
        }
    }
    else
    {
        runWiFiConfigurationServer();
    }

    delete ssid;
    delete password;
}

void WiFiSetup::logDebug(String message)
{
    Serial.print("WiFiSetup: ");
    Serial.println(message);
}

void WiFiSetup::runWiFiConfigurationServer()
{
    logDebug("Starting Access point...");

    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    String macStringLastPart = "";
    for (int i = 4; i < 6; i++)
    {
        macStringLastPart += String(mac[i], HEX);
    }

    String apName = String("Haloght-") + macStringLastPart;

    String logMessage = String("Starting access point with name '") + apName + String("'...");
    logDebug(logMessage);

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(apName.c_str(), "");
    logDebug("Access point created! Creating web server...");

    server = new WebServer(80);

    server->onNotFound(handleNotFound);
    server->on("/", handleRoot);
    server->on("/config", HTTP_POST, handlePostConfiguration);
    WebServerExtensions::registerLargeFileEndpoint("/bootstrap.min.css", "text/css; charset=utf-8", *server, bootstrapMinCss, sizeof(bootstrapMinCss) / sizeof(byte));

    server->begin();

    while (true)
    {
        server->handleClient();
        delay(1);
    }
}

void WiFiSetup::handleNotFound()
{
    logDebug("WebServer: Unknown URL called!");
    server->send(404, "text/plain", "Not found!");
}

void WiFiSetup::handleRoot()
{
    logDebug("WebServer: Root called");
    server->send_P(200, "text/html", pageWiFiSetupServerRoot);
}

void WiFiSetup::handlePostConfiguration()
{
    logDebug("WebServer: New configuration posted!");

    for (int i = 0; i < server->args(); i++)
    {
        logDebug(String("Param '") + server->argName(i) + String("' has value '") + server->arg(i) + String("'"));
    }

    String ssid = server->arg("ssid");
    String password = server->arg("password");

    if (ssid.length() == 0)
    {
        server->send(400, "text/html", pageWiFiSetupServerWrongSsid);
    }
    else
    {
        server->send(200, "text/html", pageWiFiSetupServerOk);

        Preferences preferences;
        preferences.begin(PREFERENCES_WIFI, false);

        preferences.putString(SETTING_SSID, ssid);
        preferences.putString(SETTING_PASSWORD, password);

        preferences.end();

        ESP.restart();
    }
}