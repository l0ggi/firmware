#ifndef espWebServer_header
#define espWebServer_header

#include "config.h"
#include "constants.h"
#ifdef board_ESP8266 
#ifdef USE_WEB_SERVER
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "eeprom.hpp"
#include "webserver_config.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <LittleFS.h>

void webserver_setup(void);
void webserver_loop(read_values current_vals);
bool try_to_connect_to_new_ap(String ap, String pass);
void serve_configuration_page(void);

String webserver_local_ip(void);
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
String processor(const String &var);

#endif
#endif
#endif // !espWebServer_header