#include "espWebServer.h"
#ifdef USE_WEB_SERVER
//#define FORMAT_LITTLEFS
uint8_t webserver_type = 0; // 0 = configuration server, 1 = restful interface
boolean ap_running = false;
boolean ap_ready = false;

boolean error_with_littlefs = false;

IPAddress AP_IP(WIFI_AP_IP);
IPAddress GW_IP(192, 168, 0, 1);
IPAddress SUBNET(255, 255, 255, 0);


void webserver_setup(void);
void webserver_loop(read_values current_values);

String webserver_local_ip(void)
{
    IPAddress ip = WiFi.softAPIP();
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