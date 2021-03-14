#include "espWifi.hpp"

#define IN_AP_MODE (WiFi.status() != WL_CONNECTED)

//#define FORMAT_LITTLEFS
boolean ap_running = false;
boolean ap_ready = false;

DNSServer dnsServer;

IPAddress AP_IP(WIFI_AP_IP);
IPAddress GW_IP(192, 168, 0, 1);
IPAddress SUBNET(255, 255, 255, 0);

String ssid = "";
String password = "";
uint8_t connection_error = 0; // 0 => no error, 1 => error, not configured, 2 => wrong ssid (can not be found), 3 => wrong password, 4 => auth problem

void HAL_connect(void)
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
}

void HAL_loop_handler(void)
{
    if (IN_AP_MODE)
    { // these parts are only needed if the webserver runs in AP mode
        dns_loop();
    }
    else
    {
        dns_halt();
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

void dns_halt(void)
{
    dnsServer.stop();
}

bool enable_wifi_ap(void)
{
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_GATEWAY, WIFI_AP_SUBNET);
    if (WiFi.softAP(WIFI_AP_NAME, WIFI_AP_PASS))
    {
#ifdef DEBUG_VIA_SERIAL
        Serial.print(F("DEBUG | SETUP WIFI AP | IP: "));
        Serial.println(WiFi.softAPIP());
#endif
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
#ifdef DEBUG_VIA_SERIAL
    Serial.print(F("DEBUG | WIFI | Try to connect to wifi: "));
    Serial.println(ssid);
#endif
    WiFi.persistent(persistent);
    WiFi.mode(WIFI_STA);
    if (ssid != "")
    {
        WiFi.begin(ssid, password);
    }
    else
    {
        WiFi.begin();
    }

    byte counter = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
#ifdef DEBUG_VIA_SERIAL
        Serial.print(F("DEBUG | WiFi | Trying ... "));
        Serial.println(counter);
#endif
        if (counter >= 15)
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

IPAddress get_current_ip(void)
{

    if (IN_AP_MODE)
    {
        return WiFi.softAPIP();
    }
    else
    {
        return WiFi.localIP();
    }
}

void set_wifi_connect_info(String new_ssid, String new_password)
{
    ssid = new_ssid;
    password = new_password;
}

bool get_current_server_mode(void)
{
    return IN_AP_MODE;
}