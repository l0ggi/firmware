#ifndef espWifiHeader
#define espWifiHeader

#include "config.h"
#include "constants.h"
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "setup_portal_html.hpp"
#include "espWifi.hpp"
#include "wifi_config.h"

// wifi-stack
void dns_setup(void);
void dns_loop(void);
void dns_halt(void);
bool connect_to_wifi(bool persistent, bool reset_on_fail);
bool enable_wifi_ap(void);
IPAddress get_current_ip(void);
void set_wifi_connect_info(String new_ssid, String new_password);
bool get_current_server_mode(void);

void HAL_connect(void);
void HAL_loop_handler(void);

#endif 
