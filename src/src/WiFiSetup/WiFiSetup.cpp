#include "WiFiSetup.h"

WebServer *WiFiSetup::server = NULL;

void WiFiSetup::setup(const String ssid, const String pwd)
{
    if (ssid.length() > 0)
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
}

void WiFiSetup::logDebug(String message)
{
    Serial.print("WiFiSetup: ");
    Serial.println(message);
}

void WiFiSetup::runWiFiConfigurationServer()
{
    logDebug("Starting Access point...");

    byte mac[6];
    WiFi.macAddress(mac);

    String macString = "";
    for (int i = 0; i < 6; i++)
    {
        macString += String(mac[i], HEX);
    }

    String apName = String("Haloght-") + macString;

    String logMessage = String("Starting access point with name '") + apName + String("'...");
    logDebug(logMessage);

    WiFi.softAP(apName.c_str(), "");
    logDebug("Access point created! Creating web server...");

    server = new WebServer(80);

    server->onNotFound(handleNotFound);
    server->on("/", handleRoot);
    server->on("/config", HTTP_GET, handleGetConfiguration);
    server->on("/config", HTTP_POST, handlePostConfiguration);
    server->on("/bootstrap.min.css", HTTP_GET, handleGetBootstrap);

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

void WiFiSetup::handleGetBootstrap()
{
    int chunkSize = 1500;
    int currentPosition = 0;
    int totalSize = sizeof(bootstrapMinCss) / sizeof(byte);

    server->setContentLength(CONTENT_LENGTH_UNKNOWN);
    //server->sendHeader("Content-Encoding", "gzip");
    server->send(200, "text/css", "");
    while (currentPosition < totalSize)
    {
        int endPosition = currentPosition + (chunkSize - 1) < totalSize ? currentPosition + (chunkSize - 1) : totalSize - 1;
        int currentChunkSize = endPosition - currentPosition + 1;

        // Serial.println(String("Sending byte ") + String(currentPosition) + " until " + String(endPosition) + String("!"));

        byte *currentChunk = new byte[currentChunkSize];
        for (int i = 0; i < currentChunkSize; i++)
        {
            currentChunk[i] = bootstrapMinCss[currentPosition + i];
        }

        server->sendContent((char *)currentChunk);

        delete currentChunk;
        currentPosition = endPosition + 1;
    }
}