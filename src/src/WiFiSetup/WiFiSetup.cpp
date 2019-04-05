#include "WiFiSetup.h"

void WiFiSetup::setup(const WiFiServer &wifiServer, String ssid, String pwd)
{
    if (ssid.length > 0)
    {
        // Try to connect to the access points
        WiFi.begin(ssid.c_str(), pwd.c_str());

        logDebug("Connecting to Wifi...");

        ulong startTimeConnecting = millis();
        while (WiFi.status() != WL_CONNECTED && millis() < startTimeConnecting + 10000)
        {
            delay(200);
        }

        if (WiFi.status() != WL_CONNECTED)
        {
            logDebug("Could not connect to configured access point, creating access point!");
            initializeWiFiConfigurationServer();
        }
        else
        {
            logDebug("Successfully connected to WiFi!");
            initializeWiFiConfigurationServer();
        }
    }
    else
    {
        initializeWiFiConfigurationServer();
    }
}

void WiFiSetup::logDebug(String message)
{
#ifdef DEBUG
    Serial.print("WiFiSetup: ");
    Serial.println(message);
#endif
}

void WiFiSetup::initializeWiFiConfigurationServer()
{
    byte mac[6];
    WiFi.macAddress(mac);

    String macString = "";
    for (int i = 0; i < 6; i++)
    {
        macString += String(mac[i], HEX);
    }

    String apName = String("Haloght-") + String(macString);

    String logMessage = String("Starting access point with name '") + String(apName) + String("'...");
    logDebug(logMessage);

    WiFi.softAP(apName.c_str(), "");
    logDebug("Access point created! Creating web server...");

    server = new WebServer(80);
    server->onNotFound(handleNotFound);
    server->on("/", handleRoot);
    server->on("/config", HTTP_GET, handleGetConfiguration);
    server->on("/config", HTTP_POST, handlePostConfiguration);
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