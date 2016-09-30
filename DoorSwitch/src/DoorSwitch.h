#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "user_config.h"

void setup_wifi();
void setup_OTA();
void setup_WebServer();

void handleRoot();
void handleStatus();
void handleLastMsg();
void handleNotFound();

void mqtt_callback(char* topic, byte* payload, unsigned int length);
bool mqtt_reconnect();

const char* ssid = network_ssid;
const char* password = network_password;
const char* mqtt_server = network_mqtt_server;

#define ContextType_TextPlain "text/plain"
#define ContextType_TextJson "text/json"
#define ContextType_TextHtml "text/html"
