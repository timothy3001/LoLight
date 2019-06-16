#include "WiFiSetup.h"

WebServer *WiFiSetup::server = NULL;
DNSServer *WiFiSetup::dnsServer = NULL;
const char *WiFiSetup::PREFERENCES_WIFI = "WiFi-Setup";
const char *WiFiSetup::SETTING_SSID = "ssid";
const char *WiFiSetup::SETTING_PASSWORD = "password";
char *WiFiSetup::hostnamePrefix = "WiFiSetup";
bool WiFiSetup::doRestart = false;

String WiFiSetup::getHostname()
{
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    String macStringLastPart = "";
    for (int i = 4; i < 6; i++)
    {
        macStringLastPart += String(mac[i], HEX);
    }

    String apName = String(hostnamePrefix) + String("-") + macStringLastPart;
    return apName;
}

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

    String hostname = getHostname();

    if (result)
    {
        // Try to connect to the access point
        logDebug(String("Connecting to access point with SSID '") + String(ssid) + String("' and password '") + String(password) + String("'..."));

        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        WiFi.begin(ssid, password);
        WiFi.onEvent(WiFiEventHandler);
        WiFi.setHostname(hostname.c_str());

        unsigned long startTimeConnecting = millis();
        while (WiFi.status() != WL_CONNECTED && millis() < startTimeConnecting + 30000)
        {
            delay(200);
        }

        if (WiFi.status() != WL_CONNECTED)
        {
            logDebug("Could not connect to configured access point, creating access point...");
            runWiFiConfigurationServer(hostname);
        }
        else
        {
            logDebug("Successfully connected to WiFi!");
            logDebug("Setting up mDNS...");
            if (!MDNS.begin(hostname.c_str()))
            {
                logDebug("Could not setup mDNS!");
            }
            else
            {
                logDebug(String("mDNS setup successfully! Hostname: ") + String(hostname));
                MDNS.addService("http", "tcp", 80);
            }
        }
    }
    else
    {
        runWiFiConfigurationServer(hostname);
    }

    delete ssid;
    delete password;
}

void WiFiSetup::logDebug(String message)
{
#ifdef DEBUG
    Serial.print("WiFiSetup: ");
    Serial.println(message);
#endif
}

// Workaround
void WiFiSetup::WiFiEventHandler(WiFiEvent_t event, system_event_info_t info)
{
    switch (event)
    {
    case WIFI_REASON_ASSOC_TOOMANY:
        ESP.restart();
        break;
    }
}

void WiFiSetup::runWiFiConfigurationServer(String apName)
{
    String logMessage = String("Starting access point with name '") + apName + String("'...");
    logDebug(logMessage);

    WiFi.mode(WIFI_AP);
    WiFi.disconnect();

    IPAddress ip(178, 168, 244, 1);
    IPAddress nmask(255, 255, 255, 0);
    WiFi.softAPConfig(ip, ip, nmask);
    WiFi.softAP(apName.c_str(), "");

    logDebug("Access point created! Creating web server...");

    server = new WebServer(80);
    dnsServer = new DNSServer();

    dnsServer->start(53, "*", ip);

    server->on("/", handleRoot);
    server->on("/config", HTTP_POST, handlePostConfiguration);
    WebServerExtensions::registerBootstrap(*server);
    WebServerExtensions::registerNotFound(*server);

    server->begin();

    while (!doRestart)
    {
        dnsServer->processNextRequest();
        server->handleClient();
    }

    logDebug("ESP about to restart...");
    unsigned long tsWaitForRestart = millis();
    while (tsWaitForRestart + 10000 > millis())
    {
        server->handleClient();
        dnsServer->processNextRequest();
        delay(1);
    }

    ESP.restart();
}

void WiFiSetup::handleRoot()
{
    logDebug("WebServer: Root called");
    server->send_P(200, "text/html", pageWiFiSetupServerRoot);
}

void WiFiSetup::handlePostConfiguration()
{
    logDebug("WebServer: New configuration posted!");

    String ssid = server->arg("ssid");
    String password = server->arg("password");

    if (ssid.length() == 0)
    {
        server->send(400, "text/html", pageWiFiSetupServerWrongSsid);
    }
    else
    {
        Preferences preferences;
        preferences.begin(PREFERENCES_WIFI, false);

        preferences.putString(SETTING_SSID, ssid);
        preferences.putString(SETTING_PASSWORD, password);

        preferences.end();

        server->send(200, "text/html", pageWiFiSetupServerOk);
        doRestart = true;
    }
}