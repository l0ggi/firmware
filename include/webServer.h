#ifndef WebServer_header
#define WebServer_header

#include "config.h"
#include "constants.h"
#ifdef USE_WEB_SERVER
#include <Arduino.h>

#if uController == ESP8266
#include "espWifi.hpp"
// webserver parts
void handle_save_credits(void);
void handle_webserver_setup(void);

#endif // µController = ESP8266


void webserver_setup(void);
void webserver_loop(read_values current_values);

String webserver_local_ip(void);
void handle_webserver_request(void);
String build_value_json_string(void);



#endif
#endif // 