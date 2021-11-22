#include "WiFiSetup.h"

RTC_NOINIT_ATTR bool wifiWorkaroundRtcAttr = false;

AsyncWebServer *WiFiSetup::server = NULL;
DNSServer *WiFiSetup::dnsServer = NULL;
char *WiFiSetup::hostnamePrefix = "WiFiSetup";
bool WiFiSetup::doRestart = false;
String WiFiSetup::hostnameSuffix = "";
unsigned long WiFiSetup::rebootTimestamp = 0;

String WiFiSetup::getShortMac()
{
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    String macStringLastPart = "";
    for (int i = 4; i < 6; i++)
    {
        macStringLastPart += String(mac[i], HEX);
    }

    return macStringLastPart;
}

bool WiFiSetup::readWifiSettings(char *&ssid, char *&password)
{
    Preferences preferences;

    preferences.begin(PREFERENCES_WIFI, false);

    String ssidString = preferences.getString(SETTING_SSID, String(""));
    hostnameSuffix = preferences.getString(SETTING_HOSTNAME_SUFFIX);

    if (hostnameSuffix.length() == 0)
    {
        hostnameSuffix = getShortMac();
        preferences.putString(SETTING_HOSTNAME_SUFFIX, hostnameSuffix);
    }

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

    String hostname = String(hostnamePrefix) + String("-") + hostnameSuffix;

    if (result)
    {
        // Try to connect to the access point
        logDebug(String("Connecting to access point with SSID '") + String(ssid) + String("' and password '") + String(password) + String("'..."));

        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);
        if (strlen(password) > 0)
            WiFi.begin(ssid, password);
        else
            WiFi.begin(ssid);
        WiFi.setHostname(hostname.c_str());
        WiFi.onEvent(WiFiEventHandler);

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
        logDebug(String("WORKAROUND for wifi happening! Rebooting..."));
        Serial.flush();

        wifiWorkaroundRtcAttr = true;

        delay(500);

        ESP.restart();
        break;
    }
}

bool WiFiSetup::wasWorkaroundExecuted()
{
    if (rtc_get_reset_reason(0) == 12 && wifiWorkaroundRtcAttr)
        return true;
    return false;
}

void WiFiSetup::runWiFiConfigurationServer(String apName)
{
    String logMessage = String("Starting access point with name '") + apName + String("'...");
    logDebug(logMessage);
    rebootTimestamp = millis();

    WiFi.mode(WIFI_AP);
    WiFi.disconnect();

    IPAddress ip(192, 168, 4, 1);
    IPAddress nmask(255, 255, 255, 0);
    WiFi.softAPConfig(ip, ip, nmask);
    WiFi.softAP(apName.c_str(), "");
    WiFi.softAPConfig(ip, ip, nmask);

    logDebug("Access point created! Creating web server...");

    server = new AsyncWebServer(80);
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
        if (millis() > rebootTimestamp + timeBeforeReboot)
            doRestart = true;
        delay(1);
    }

    logDebug("ESP about to restart...");
    unsigned long tsWaitForRestart = millis();
    while (tsWaitForRestart + 10000 > millis())
    {
        dnsServer->processNextRequest();
        delay(1);
    }

    ESP.restart();
}

void WiFiSetup::handleRoot(AsyncWebServerRequest *request)
{
    WiFiSetup::rebootTimestamp = millis(); // Resetting timestamp so that it won't restart
    logDebug("WebServer: Root called");
    request->send_P(200, "text/html", pageWiFiSetupServerRoot);
}

void WiFiSetup::handlePostConfiguration(AsyncWebServerRequest *request)
{
    logDebug("WebServer: New configuration posted!");

    if (!request->hasArg("ssid") || request->arg("ssid").length() == 0)
    {
        request->send(400, "text/html", pageWiFiSetupServerWrongSsid);
    }
    else
    {
        String ssid = request->arg("ssid");
        String password = request->hasArg("password") ? request->arg("password") : String("");

        Preferences preferences;
        preferences.begin(PREFERENCES_WIFI, false);

        preferences.putString(SETTING_SSID, ssid);
        preferences.putString(SETTING_PASSWORD, password);

        preferences.end();

        request->send(200, "text/html", pageWiFiSetupServerOk);
        doRestart = true;
    }
}
