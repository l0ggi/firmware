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


void webserver_setup(void);
void webserver_loop(read_values current_values);

String webserver_local_ip(void);

#endif
#endif
#endif // !espWebServer_header