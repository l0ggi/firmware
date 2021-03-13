#include "espWebServer.h"
#ifdef USE_WEB_SERVER

#define IN_AP_MODE (WiFi.status() != WL_CONNECTED)

//#define FORMAT_LITTLEFS
boolean ap_running = false;
boolean ap_ready = false;

boolean error_with_littlefs = false;

ESP8266WebServer server(80);
DNSServer dnsServer;

IPAddress AP_IP(WIFI_AP_IP);
IPAddress GW_IP(192, 168, 0, 1);
IPAddress SUBNET(255, 255, 255, 0);

read_values current_values_esp;

String ssid = "";
String password = "";
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
    server.on("/", HTTP_GET, handle_webserver_request);
    server.onNotFound(handle_webserver_request); // handles when not connected to a wifi the settings page, otherwise returns the last sampled json
    server.begin();
}

void webserver_loop(read_values current_values)
{
    current_values_esp = current_values;
    if (IN_AP_MODE)
    { // these parts are only needed if the webserver runs in AP mode
        dns_loop();
    }
    else
    {
        dns_halt();
    }
    server.handleClient();
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

void handle_save_credits(void)
{
    if (server.hasArg("WIFI_SSID") && server.hasArg("WIFI_PW"))
    {
#ifdef DEBUG_VIA_SERIAL
        Serial.println(F("DEBUG | WEBSERVER | Saving Credits: "));
#endif
        ssid = server.arg("WIFI_SSID");
        password = server.arg("WIFI_PW");
#ifdef DEBUG_VIA_SERIAL
        Serial.print(F("SSID: "));
        Serial.print(ssid);
        Serial.print(F("PW: "));
        Serial.println(password);
        Serial.print(F("Plain request: "));
        Serial.println(server.arg("plain"));
        String message = "Arguments: ";
        for (uint8_t i = 0; i < server.args(); i++)
        {
            message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
            Serial.println(message);
        }

#endif
        server.send(200, "text/plain", ANSWER_GOT_CREDITS);
        connect_to_wifi(true, true);
    }
}

void handle_webserver_request(void)
{
    if (IN_AP_MODE)
    {
#ifdef DEBUG_VIA_SERIAL
        Serial.println(F("DEBUG | WEBSERVER | Sending captive setup portal"));
#endif
        server.send(200, "text/html", captive_portal);
    }
    else
    {
#ifdef DEBUG_VIA_SERIAL
        Serial.println(F("DEBUG | WEBSERVER | Regular handler"));
#endif
        server.send(200, "text/json", build_value_json_string());
    }
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

String build_value_json_string(void)
{

    /**
     * Arduino has no float-functions in sprintf.. we need do build something on our own:
     * */

    String temp = String(current_values_esp.values.temperature, '\002');
    String humid = String(current_values_esp.values.humidity, '\000');

    char curTemp[sizeof("00.0")];
    char curHumid[sizeof("00")];
    temp.toCharArray(curTemp, sizeof("00.0"));
    humid.toCharArray(curHumid, sizeof("00"));

#ifdef DEBUG_VIA_SERIAL
        Serial.print(F("DEBUG | WiFi | Sending Vals: Temp: "));
        Serial.print(temp);
        Serial.print(" Humid: ");
        Serial.println(humid);
        
#endif

    String values = "{";
    values += "\"temperature\":";
    values += temp;
    values += ",\"humidity\":";
    values += humid;
    values += "}";
    return values;
}

// BUG: if all 4x3 digits are used, the display overflows
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