#include "espWebServer.h"
#ifdef USE_WEB_SERVER
//#define FORMAT_LITTLEFS
wifi_settings current_settings = {};
uint8_t webserver_type = 0; // 0 = configuration server, 1 = restful interface
boolean ap_running = false;
boolean ap_ready = false;

boolean error_with_littlefs = false;

IPAddress AP_IP(WIFI_AP_IP);
IPAddress GW_IP(192, 168, 0, 1);
IPAddress SUBNET(255, 255, 255, 0);

AsyncWebServer server(80); // handles the async webserver instance
AsyncWebSocket ws("/socket");
AsyncEventSource events("/events");

DNSServer dns; // captive dns server

read_values current_vars; // store the current temperature values to use here

#ifdef DEBUG_VIA_SERIAL
boolean ap_ready_msg_sent = false;
boolean config_server_msg_sent = false;
#endif

void webserver_setup(void)
{
    read_wifi_settings_from_eeprom(&current_settings);
    if (!current_settings.configured) // if the wifi-server is not yet configured we start the configuration. single html page, input for wifi-ap and password.
    {
        webserver_type = 0;
#ifdef DEBUG_VIA_SERIAL
        Serial.println("DEBUG | WIFI_AP | Webserver Type 0 starting");
#endif
    }
    else
    {
        webserver_type = 1;
#ifdef DEBUG_VIA_SERIAL
        Serial.println("DEBUG | WIFI_AP | Webserver Type 1 starting");
#endif
    }

    // init the file system
    // only needed in webserver mode, in client mode data won't get stored and we do not need to display websites
    if (!LittleFS.begin())
    {
#ifdef FORMAT_LITTLEFS
        LittleFS.format();
        Serial.println("DEBUG | LittleFS | Formatted filesystem");
#endif

#ifdef DEBUG_VIA_SERIAL
        Serial.println("Error while starting LittleFS file system. Going on anyways");
#endif
        error_with_littlefs = true; // set a flag for filesystem error to use fallback options in webserver handling
    }

    // Start DNS Service
    MDNS.addService("http", "tcp", 80);

    // websockets
    ws.onEvent(onWsEvent);
    server.addHandler(&ws); // add websocket handler to webserver

    // events
    events.onConnect([](AsyncEventSourceClient *client) {
        client->send("Welcome!", NULL, millis(), 1000);
    });
    server.addHandler(&events);
}

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
        //request->addInterestingHeader("ANY");
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request)
    {
        AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/setup.html");
        /*
        AsyncResponseStream *response = request->beginResponseStream("text/html");
        response->print("<!DOCTYPE html><html><head><title>Captive Portal</title></head><body>");
        response->print("<p>This is out captive portal front page.</p>");
        response->printf("<p>You were trying to reach: http://%s%s</p>", request->host().c_str(), request->url().c_str());
        response->printf("<p>Try opening <a href='http://%s'>this link</a> instead</p>", WiFi.softAPIP().toString().c_str());
        response->print("</body></html>");
        */
        request->send(response);
    }
};

String processor(const String &var)
{
    if (var == "TEMP")
        return String(current_vars.values.temperature);
    if (var == "HUMIDITY")
        return String(current_vars.values.humidity);
    if (var == "TIMESTAMP")
        return String(current_vars.timestamp.hours) + ":" + String(current_vars.timestamp.minutes) + ":" + String(current_vars.timestamp.seconds) + "." + String(current_vars.timestamp.millis);
}

void webserver_loop(read_values current_vals)
{
    if (!error_with_littlefs)
    {
        // store the current values to local values
        current_vars = current_vals;
        if (webserver_type == 0)
        { // configuration webserver. start the config server
            serve_configuration_page();
#ifdef DEBUG_VIA_SERIAL
            if (!config_server_msg_sent)
            {
                config_server_msg_sent = true;
                Serial.println("DEBUG | WIFI_AP | serving config server");
            }
#endif
        }
        else
        {
            Serial.println("Configured, but nothing to do..");
        }

        // websockets: need to clean disconnected clients to save heap
        ws.cleanupClients();
    }
    else
    {
        Serial.println("ERROR | LittleFS | Error with LittleFS. Could not Start filesystem. Webserver disabled");
    }
}

bool try_to_connect_to_new_AP(String ap, String pass)
{
    WiFi.begin(ap, pass);
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        counter++;
        if (counter > 10)
        {
            return false;
        }
    }
    return true;
}

void serve_configuration_page(void)
{
    // setup a wifi ap
    if (!ap_running)
    {
        WiFi.softAPConfig(AP_IP, GW_IP, SUBNET);
        ap_ready = WiFi.softAP(WIFI_AP_NAME + WiFi.macAddress().substring(-4, 4), WIFI_AP_PASS);

        // -------------------------------------- config setup --------------------------------------
        // setup captive DNS server

        dns.start(53, "*", WIFI_AP_IP);
        // Request the free heap size on this endpoint. useful while debugging. might be logged later for in production debugging. will see
        server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(200, "text/plain", String(ESP.getFreeHeap()));
        });
        //server.serveStatic("/", LittleFS, "/").setDefaultFile("console.htm");
        // configuration webpage stored in LittleFS
        // delivered by captive portal on connection
        server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); //only when requested from AP
        server.begin();
        ap_running = true;
        // -------------------------------------- config setup --------------------------------------
    }
    if (ap_ready)
    {   // start the handler if the access point is ready
        // @TODO: add search for available networks and post them to a dropdown menu
#ifdef DEBUG_VIA_SERIAL
        if (!ap_ready_msg_sent)
        {
            ap_ready_msg_sent = true;
            Serial.println("DEBUG | WIFI_AP | AP Ready");
        }
#endif
        dns.processNextRequest();

        /*
        server.on("/configure", HTTP_POST, [](AsyncWebServerRequest *request) {
            String ap;   // sent access point
            String pass; // sent password
            String err;  // eventual error return string
            if (request->hasParam("WIFI_AP", true))
            {
                ap = request->getParam("WIFI_AP", true)->value();
            }
            else
            {
                err += "No WIFI_AP sent! \n";
            }
            if (request->hasParam("WIFI_PASS", true))
            {
                pass = request->getParam("WIFI_PASS", true)->value();
            }
            else
            {
                err += "No WIFI_PASS sent! \n";
            }

            // try to connect to the supplied network before answering the request
            if (/*!try_to_connect_to_new_ap(ap, pass)true)
            {
                err += "Invalid Network!";
            }
            if (sizeof(err) > 0)
            {
                request->send(500, "text/plain", "Errors: " + err);
            }
            else
            {
                request->send(200, "text/plain", "Connection to new AP possible");

                wifi_settings new_wifi_settings;
                new_wifi_settings.ssid = ap;
                new_wifi_settings.password = pass;
                new_wifi_settings.configured = true;

                write_wifi_settings_to_eeprom(&new_wifi_settings);
            }
            
        });*/
    }
}

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

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
        client->printf("Hello Client %u :)", client->id());
        client->ping();
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.printf("ws[%s][%u] disconnect\n", server->url(), client->id());
    }
    else if (type == WS_EVT_ERROR)
    {
        Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
    }
    else if (type == WS_EVT_PONG)
    {
        Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len) ? (char *)data : "");
    }
    else if (type == WS_EVT_DATA)
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        String msg = "";
        if (info->final && info->index == 0 && info->len == len)
        {
            //the whole message is in a single frame and we got all of it's data
            Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT) ? "text" : "binary", info->len);

            if (info->opcode == WS_TEXT)
            {
                for (size_t i = 0; i < info->len; i++)
                {
                    msg += (char)data[i];
                }
            }
            else
            {
                char buff[3];
                for (size_t i = 0; i < info->len; i++)
                {
                    sprintf(buff, "%02x ", (uint8_t)data[i]);
                    msg += buff;
                }
            }
            Serial.printf("%s\n", msg.c_str());

            if (info->opcode == WS_TEXT)
                client->text("I got your text message");
            else
                client->binary("I got your binary message");
        }
        else
        {
            //message is comprised of multiple frames or the frame is split into multiple packets
            if (info->index == 0)
            {
                if (info->num == 0)
                    Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
                Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
            }

            Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT) ? "text" : "binary", info->index, info->index + len);

            if (info->opcode == WS_TEXT)
            {
                for (size_t i = 0; i < len; i++)
                {
                    msg += (char)data[i];
                }
            }
            else
            {
                char buff[3];
                for (size_t i = 0; i < len; i++)
                {
                    sprintf(buff, "%02x ", (uint8_t)data[i]);
                    msg += buff;
                }
            }
            Serial.printf("%s\n", msg.c_str());

            if ((info->index + len) == info->len)
            {
                Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
                if (info->final)
                {
                    Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT) ? "text" : "binary");
                    if (info->message_opcode == WS_TEXT)
                        client->text("I got your text message");
                    else
                        client->binary("I got your binary message");
                }
            }
        }
    }
}

#endif