#include "WiFiSetup.h"

WebServer *WiFiSetup::server = NULL;

bool WiFiSetup::readWifiSettings(String *ssid, String *pwd)
{
    Preferences preferences;

    preferences.begin("Wifi-Setup", true);

    ssid = new String(preferences.getString("ssid", String("")));

    if (ssid->length() == 0)
        return false;

    pwd = new String(preferences.getString("pwd", String("")));

    return true;
}

void WiFiSetup::setup()
{
    String *ssid = 0;
    String *pwd = 0;

    if (readWifiSettings(ssid, pwd))
    {
        // Try to connect to the access point
        logDebug(String("Connecting to access point with SSID '") + String(ssid->c_str()) + String("'..."));
        WiFi.begin(ssid->c_str(), pwd->c_str());

        ulong startTimeConnecting = millis();
        while (WiFi.status() != WL_CONNECTED && millis() < startTimeConnecting + 10000)
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
    delete pwd;
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

    WiFi.softAP(apName.c_str(), "");
    logDebug("Access point created! Creating web server...");

    server = new WebServer(80);

    server->onNotFound(handleNotFound);
    server->on("/", handleRoot);
    server->on("/config", HTTP_GET, handleGetConfiguration);
    server->on("/config", HTTP_POST, handlePostConfiguration);
    WebServerExtensions::registerLargeFileEndpoint("/bootstrap.min.css", "text/css", *server, bootstrapMinCss, sizeof(bootstrapMinCss) / sizeof(byte));

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
    String body = server->arg("plain");
}

void WiFiSetup::handleGetConfiguration()
{
    logDebug("WebServer: Current configuration called!");
}