#ifndef UNIT_TEST
#ifndef OSX

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <status.h>
#include "./bridgeWebServer/bridgeWebServer.h"
#include "./bridgeMQTTClient/bridgeMQTTClient.h"

#include "user_config.h"

void setup_wifi();
void setup_OTA();

void led_set(bool on);
void led_toggle();

const char* ssid = network_ssid;
const char* password = network_password;
const char* mqtt_server = network_mqtt_server;

#endif // OSX
#endif // UNIT_TEST
