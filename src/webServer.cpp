#include "webServer.h"
#ifdef USE_WEB_SERVER

#if uController == ESP8266
ESP8266WebServer server(80);
#include "espWifi.hpp"
#endif

IPAddress current_ip;
read_values current_values_esp;

bool IN_AP_MODE;

void webserver_setup(void)
{
    HAL_connect();
    server.on("/store", HTTP_POST, handle_save_credits);
    server.on("/", HTTP_GET, handle_webserver_request);
    server.on("/setup", HTTP_GET, handle_webserver_setup);
    server.onNotFound(handle_webserver_request); // handles when not connected to a wifi the settings page, otherwise returns the last sampled json
    server.begin();
}

void webserver_loop(read_values current_values)
{
    IN_AP_MODE = get_current_server_mode();
    current_values_esp = current_values;
    HAL_loop_handler();
    server.handleClient();
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

#ifdef uController == ESP8266
void handle_save_credits(void)
{
    if (server.hasArg("WIFI_SSID") && server.hasArg("WIFI_PW"))
    {
#ifdef DEBUG_VIA_SERIAL
        Serial.println(F("DEBUG | WEBSERVER | Saving Credits: "));
#endif
        set_wifi_connect_info(server.arg("WIFI_SSID"), server.arg("WIFI_PW"));
#ifdef DEBUG_VIA_SERIAL
        Serial.print(F("SSID: "));
        Serial.print(server.arg("WIFI_SSID"));
        Serial.print(F("PW: "));
        Serial.println(server.arg("WIFI_PW"));
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
#endif

void handle_webserver_setup(void)
{
    server.send(200, "text/html", captive_portal);
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

#ifdef HAS_LIGHT_SENSOR
    String light = String(current_values_esp.values.light, '\000');
#endif
#ifdef HAS_NOX_SENSOR
    String nox = String(current_values_esp.values.nox, '\000');
#endif
#ifdef HAS_CO2_SENSOR
    String nox = String(current_values_esp.values.co2, '\000');
#endif

#ifdef DEBUG_VIA_SERIAL
    Serial.print(F("DEBUG | WiFi | Sending Vals: Temp: "));
    Serial.print(temp);

#ifdef HAS_LIGHT_SENSOR
    Serial.print(", Light: ");
    Serial.print(light);
#endif
#ifdef HAS_NOX_SENSOR
    Serial.print(", NOX:  ");
    Serial.print(nox);
#endif
#ifdef HAS_CO2_SENSOR
    Serial.print(", CO2: ");
    Serial.print(co2);
#endif
    Serial.print(" Humid: ");
    Serial.println(humid);
#endif

    String values = "{";
    values += "\"temperature\":";
    values += temp;
    values += ",\"humidity\":";
    values += humid;
#ifdef HAS_LIGHT_SENSOR
    values += ",\"light\":";
    values += light;
#endif
#ifdef HAS_NOX_SENSOR
    values += ",\"nox\":";
    values += nox;
#endif
#ifdef HAS_CO2_SENSOR
    values += ",\"co2\":";
    values += co2;
#endif
    values += "}";
    return values;
}

// BUG: if all 4x3 digits are used, the display overflows
String webserver_local_ip(void)
{
    current_ip = get_current_ip();

    String current = "";
    current += current_ip[0];
    current += ".";
    current += current_ip[1];
    current += ".";
    current += current_ip[2];
    current += ".";
    current += current_ip[3];
    return current;
}
#endif