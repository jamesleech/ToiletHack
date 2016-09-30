#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <CustomOTA.h>
#include <PubSubClient.h>

#include "doorSwitch.h"

MDNSResponder mdns;
bool mdnsStarted = false;

ESP8266WebServer server(80);

char led_status = '0';

WiFiClient espClient;
PubSubClient client(espClient);
long mqtt_lastReconnectAttempt = 0;
long mqtt_lastMsgTimer = 0;
int mqtt_value = 0;
char mqtt_msg[50];
char mqtt_received[50];

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //"Connection Failed! Rebooting
    delay(5000);
    ESP.restart();
  }
}

void setup_OTA() {

  // Port defaults to 8266
  // CustomOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // CustomOTA.setHostname("myesp8266");

  // No authentication by default
  // CustomOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // CustomOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  CustomOTA.onStart([]() {
    String type;
    if (CustomOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    //Serial.println("Start updating " + type);
  });
  CustomOTA.onEnd([]() {
    //Serial.println("\nEnd");
  });
  CustomOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  CustomOTA.onError([](ota_error_t error) {
    /*Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");*/
  });

  CustomOTA.begin();
}

void setup_WebServer() {
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/lastMsg", handleLastMsg);
  server.onNotFound(handleNotFound);
  server.begin();
}

void setup_MQTT() {
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(BUILTIN_LED, HIGH);
  digitalWrite(2, HIGH);

  setup_wifi();

  mdnsStarted = mdns.begin("doorSwitch", WiFi.localIP());

  setup_OTA();
  setup_WebServer();
  setup_MQTT();
  digitalWrite(BUILTIN_LED, LOW);
  digitalWrite(2, HIGH);
}

void loop() {
  CustomOTA.handle();
  server.handleClient();

  if (!client.connected()) {
    long now = millis();
    //only try and reconnect every 5 seconds
    if (now - mqtt_lastReconnectAttempt > 5000) {
      mqtt_lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqtt_reconnect()) {
        mqtt_lastReconnectAttempt = 0;
      }
    }
  } else {
    //mqtt subscribe
    client.loop();

    //mqtt publish
    //publish a status message every 2 seconds
    long now = millis();
    if (now - mqtt_lastMsgTimer > 2000) {
      mqtt_lastMsgTimer = now;
      ++mqtt_value;
      //snprintf_P(char *str, size_t strSize, const char *formatP, ...)
      snprintf (mqtt_msg, 50, "hello world #%ld", mqtt_value);
      client.publish("outTopic", mqtt_msg);
    }
  }
}

void handleRoot() {
  server.send(200, ContextType_TextHtml,
    "<!DOCTYPE html><html><head><title>Toilet Hack Node</title></head><body><p>I'm a node, one of many.</p></body></html>");
}

void handleStatus() {

  if(mdnsStarted){
    // server.send(200, ContextType_TextJson,       "{\"mdns\":\"true\", \"ip\":\"todo\"}");
  } else {
    //server.send(200, ContextType_TextJson,       "{\"mdns\":\"false\", \"ip\":\"todo\"}");
  }

  char status[50];
  snprintf(status, 50,
    "{\"mqttValue\":\"%ld\",\"LED\":\"%ld\",\"ip\":\"todo\"}",
    mqtt_value, led_status);

  server.send(200, ContextType_TextJson, status);
}

void handleLastMsg() {
    server.send(200, ContextType_TextPlain, mqtt_received);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  //only care about the first 50 chars
  if(length > 50) {
    length = 50;
  }

  //assume the bytes in the payload are chars
  for (int i = 0; i < length; i++) {
    mqtt_received[i] = (char)payload[i];
  }

  // Switch on the LED if an 1 was received as first character
  if (mqtt_received[0] == '1') {
    led_status = LOW;
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    digitalWrite(2,HIGH);
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    led_status = HIGH;
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    digitalWrite(2,LOW);
  }
}

bool mqtt_reconnect() {
  if (client.connect("ESP8266Client")) {
    // Once connected, publish an announcement...
    client.publish("outTopic","hello world");
    // ... and resubscribe
    client.subscribe("inTopic");
  }
  return client.connected();
}
