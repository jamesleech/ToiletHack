#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <CustomOTA.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "user_config.h"

char led_status = 0;
WiFiClient espClient;
PubSubClient client(espClient);

long mqtt_lastReconnectAttempt = 0;
long mqtt_lastMsgTimer = 0;
long mqtt_counter = 0;

void setup_wifi();
void setup_OTA();
void setup_MQTT();

void led_set(bool on);

void mqtt_callback(char* topic, byte* payload, unsigned int length);
bool mqtt_reconnect();
void mqtt_publishConnected();
void mqtt_publishStatus();

const char* ssid = network_ssid;
const char* password = network_password;
const char* mqtt_server = network_mqtt_server;

#define ContextType_TextPlain "text/plain"
#define ContextType_TextJson "text/json"
#define ContextType_TextHtml "text/html"

#define MQTT_TOPIC "ToiletHack"
