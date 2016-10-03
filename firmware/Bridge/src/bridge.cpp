#ifndef UNIT_TEST
#ifndef OSX

#include <PubSubClient.h>
#include "bridge.h"

bool led_status = false;
Status status(ESP.getChipId());
MDNSResponder mdns;
ESP8266WebServer* webServer;
BridgeWebServer* bridge;

WiFiClient espClient;
PubSubClient pubSubClient(espClient);
BridgeMQTTClient* mqtt;
long mqtt_lastMsgTimer = 0;

void setup() {
  pinMode(0, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  led_set(true);

  setup_wifi();
  setup_OTA();
  //TODO: if OTA only button press don't do anything else.
  // this should allow broken updates to be updated :)

  mdns.begin("hub1", WiFi.localIP());
  webServer = new ESP8266WebServer(WiFi.softAPIP()); //WiFi.softAPIP() //WiFi.localIP()
  bridge = new BridgeWebServer(webServer);
  mqtt = new BridgeMQTTClient(&pubSubClient);

  led_set(false);

  WiFi.localIP().toString().toCharArray(status.NodeIp, 17);
}

void loop() {

  ArduinoOTA.handle();
  webServer->handleClient();
  mqtt->Handle();

  //DEBUG: publish status every 2 seconds
  long now = millis();
  if (now - mqtt_lastMsgTimer > 2000) {
    mqtt_lastMsgTimer = now;
    status.State = (digitalRead(0) == 1);
    mqtt->PublishStatus(&status);
  }
}

void setup_wifi() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  WiFi.softAP("THAP-01","ToiletHackPassword");

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //"Connection Failed! Rebooting
    delay(5000);
    ESP.restart();
  }
}

void led_set(bool on) {
  led_status = on;
  if (on) {
    digitalWrite(LED_BUILTIN, LOW); // but actually the LED is on; this is it is acive low on the ESP-01)
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void led_toggle() {
  led_set(!led_status);
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

  ArduinoOTA.onStart([]() {
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    led_set(true);
  });

  ArduinoOTA.onEnd([]() {
    led_set(false);
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    led_toggle();
  });

  ArduinoOTA.onError([](ota_error_t error) {
    if (error == OTA_AUTH_ERROR) mqtt->Publish("OTA update: Auth Failed");
    else if (error == OTA_BEGIN_ERROR) mqtt->Publish("OTA update: Begin Failed");
    else if (error == OTA_CONNECT_ERROR) mqtt->Publish("OTA update: Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) mqtt->Publish("OTA update: Receive Failed");
    else if (error == OTA_END_ERROR) mqtt->Publish("OTA update: End Failed");
  });

  ArduinoOTA.begin();
}

#endif // OSX
#endif // UNIT_TEST
