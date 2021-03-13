#ifndef espWebServer_header
#define espWebServer_header

#include "config.h"
#include "constants.h"
#ifdef board_ESP8266 
#ifdef USE_WEB_SERVER
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "eeprom.hpp"
#include "webserver_config.h"
#include "setup_portal_html.hpp"


// wifi-stack
void webserver_setup(void);
void webserver_loop(read_values *current_values);
void dns_setup(void);
void dns_loop(void);
bool connect_to_wifi(bool persistent, bool reset_on_fail);
bool enable_wifi_ap(void);
String webserver_local_ip(void);



// webserver parts
void handle_save_credits(void);
void handle_webserver_request(void);

#endif
#endif
#endif // !espWebServer_header