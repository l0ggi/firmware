#include "espWebServer.h"
#ifdef USE_WEB_SERVER

#define IN_AP_MODE !(WiFi.status() != WL_CONNECTED)

//#define FORMAT_LITTLEFS
boolean ap_running = false;
boolean ap_ready = false;

boolean error_with_littlefs = false;

ESP8266WebServer server(80);
DNSServer dnsServer;

IPAddress AP_IP(WIFI_AP_IP);
IPAddress GW_IP(192, 168, 0, 1);
IPAddress SUBNET(255, 255, 255, 0);

read_values *current_values_ptr; // pointer to the current stored value field

char *ssid = "";
char *password = "";
uint8_t connection_error = 0; // 0 => no error, 1 => error, not configured, 2 => wrong ssid (can not be found), 3 => wrong password, 4 => auth problem

void webserver_setup(void)
{
    // try to connect to configured wifi
    if (!connect_to_wifi(false, false))
    {
        enable_wifi_ap();
    }
    if (IN_AP_MODE)
    {
        dns_setup();
    }
    server.on("/store", HTTP_POST, handle_save_credits);
    server.onNotFound(handle_webserver_request); // handles when not connected to a wifi the settings page, otherwise returns the last sampled json
}
void webserver_loop(read_values *current_values)
{
    current_values_ptr = current_values;
    if (IN_AP_MODE)
    { // these parts are only needed if the webserver runs in AP mode
        dns_loop();
    }
}

void dns_setup(void)
{
    dnsServer.start(DNS_PORT, "*", WIFI_AP_IP);
}

void dns_loop(void)
{
    dnsServer.processNextRequest();
}

void handle_save_credits(void)
{
    if (server.hasArg("WIFI_SSID") && server.hasArg("WIFI_PW"))
    {
        strcpy(ssid, server.arg(0).c_str());
        strcpy(password, server.arg(1).c_str());
        server.send(200, "text/plain", ANSWER_GOT_CREDITS);
        connect_to_wifi(true, true);
    }
}

void handle_webserver_request(void)
{
    if (IN_AP_MODE)
    {
        server.send(200, "text/html", captive_portal);
    }
    else
    {
        server.send(404, "text/plain", "Not Found");
    }
}

bool enable_wifi_ap(void)
{
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(WIFI_AP_IP, IPAddress(0, 0, 0, 0), IPAddress(255, 255, 0, 0));

    if (WiFi.softAP(WIFI_AP_NAME, WIFI_AP_PASS))
    {
        return true;
    }
    return false;
}

// This function connects to a configured wifi AP
// if no AP is configured, function returns false
// if the AP is not reachable, function returns false and the connection error uint gets set
// @TODO: implement static IP handler
bool connect_to_wifi(bool persistent, bool reset_on_fail)
{
    WiFi.persistent(persistent);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    byte counter = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (counter >= 9)
        {
            connection_error = 4;
            if (reset_on_fail)
            {
                ESP.reset(); // reset the device. should work for first tries, possible future implementation tbd
            }
            return false;
        }
        delay(500);
        counter++;
    }
    return true;
}

String webserver_local_ip(void)
{
    IPAddress ip;
    if (IN_AP_MODE)
    {
        ip = WiFi.softAPIP();
    }
    else
    {
        ip = WiFi.localIP();
    }
    String current = "";
    current += ip[0];
    current += ".";
    current += ip[1];
    current += ".";
    current += ip[2];
    current += ".";
    current += ip[3];
    return current;
}
#endif