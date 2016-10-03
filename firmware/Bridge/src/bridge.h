#ifndef UNIT_TEST
#ifndef OSX

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <status.h>

#include "user_config.h"

bool led_status = false;

Status status(Status_Json_Len);

MDNSResponder mdns;
ESP8266WebServer webServer(80);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

long mqtt_lastReconnectAttempt = 0;
long mqtt_lastMsgTimer = 0;

void setup_wifi();
void setup_OTA();
void setup_WebServer();
void setup_MQTT();

void led_set(bool on);
void led_toggle();

void mqtt_callback(char* topic, byte* payload, unsigned int length);
bool mqtt_reconnect();
void mqtt_publishConnected();
void mqtt_publishStatus();

void handleRoot();
void handleStatus();
void handleNotFound();

const char* ssid = network_ssid;
const char* password = network_password;
const char* mqtt_server = network_mqtt_server;

#define ContextType_TextPlain "text/plain"
#define ContextType_TextJson "text/json"
#define ContextType_TextHtml "text/html"

#define MQTT_TOPIC "ToiletHack"

#endif // OSX
#endif // UNIT_TEST
