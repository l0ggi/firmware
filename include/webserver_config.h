#include "config.h"

#ifdef USE_WEB_SERVER
#ifndef WEBSERVER_CONFIG_HEADER
#define WEBSERVER_CONFIG_HEADER

#define WIFI_AP_NAME "l0ggi"
#define WIFI_AP_PASS ""
#define WIFI_AP_IP IPAddress(192, 168, 0, 1)
#define WIFI_AP_GATEWAY IPAddress(192, 168, 0, 1)
#define WIFI_AP_SUBNET IPAddress(255, 255, 255, 0)
#define DNS_PORT 53

#endif
#endif