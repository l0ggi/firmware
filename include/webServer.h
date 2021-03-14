#ifndef WebServer_header
#define WebServer_header

#include "config.h"
#include "constants.h"
#include <Arduino.h>
#ifdef USE_WEB_SERVER

#if uController == ESP8266
#include "espWifi.hpp"
#include <ESP8266WebServer.h>
// webserver parts
void handle_save_credits(void);
void handle_webserver_setup(void);

#endif // µController = ESP8266


void webserver_setup(void);
void webserver_loop(read_values current_values);

String build_value_json_string(void);
String webserver_local_ip(void);
String generate_uuid(void);

void handle_webserver_request(void);
void handle_webserver_register(void);
void handle_webserver_setup(void);





#endif
#endif // 