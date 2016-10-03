#ifndef UNIT_TEST
#ifndef OSX

#include "bridge.h"

bool led_status = false;
Status status(Status_Json_Len);
MDNSResponder mdns;
ESP8266WebServer webServer(80);
BridgeWebServer* bridge;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

char status_json[Status_Json_Len]; //alloc this once and keep it.

void setup() {
  pinMode(0, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  led_set(true);

  setup_wifi();

  mdns.begin("hub1", WiFi.localIP());

  bridge = new BridgeWebServer(&webServer);

  setup_OTA();
  setup_MQTT();

  led_set(false);
}

void loop() {
  ArduinoOTA.handle();
  webServer.handleClient();

  //mqtt connected?
  if (!mqttClient.connected()) {
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
    //do mqtt subscribe
    mqttClient.loop();

    //mqtt publish
    //publish a status message every 2 seconds
    long now = millis();
    if (now - mqtt_lastMsgTimer > 2000) {
      mqtt_lastMsgTimer = now;
      mqtt_publishStatus();
    }
  }
}

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
    if (error == OTA_AUTH_ERROR) mqttClient.publish(MQTT_TOPIC, "OTA update: Auth Failed");
    else if (error == OTA_BEGIN_ERROR) mqttClient.publish(MQTT_TOPIC, "OTA update: Begin Failed");
    else if (error == OTA_CONNECT_ERROR) mqttClient.publish(MQTT_TOPIC, "OTA update: Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) mqttClient.publish(MQTT_TOPIC, "OTA update: Receive Failed");
    else if (error == OTA_END_ERROR) mqttClient.publish(MQTT_TOPIC, "OTA update: End Failed");
  });

  ArduinoOTA.begin();
}

void setup_MQTT() {
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(mqtt_callback);
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

void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  char mqtt_received[length];
  //assume the bytes in the payload are chars
  for (int i = 0; i < length; i++) {
    mqtt_received[i] = (char)payload[i];
  }

  led_set(mqtt_received[0] == '1'); // Switch on the LED if a 1 was received as first character

  mqttClient.publish(MQTT_TOPIC, "Got it!");
}

bool mqtt_reconnect() {
  if (mqttClient.connect("ToiletHackNode0")) {
    mqtt_publishConnected(); // Once connected, publish an announcement...

    char mqttchannel[30];
    snprintf(mqttchannel, 30, "ToiletHack.Node.%ld", ESP.getChipId());

    mqttClient.subscribe(mqttchannel, 1);
  }
  return mqttClient.connected();
}

void mqtt_publishStatus() {
  status.State = digitalRead(0) == 1;
  status.toJson(status_json);

  mqttClient.publish(MQTT_TOPIC, status_json);
}

void mqtt_publishConnected() {
  led_set(true);
  char connected[50];

  uint32_t id = ESP.getChipId();

  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"Vcc\":\"%ld\"}",
    id,
    ESP.getVcc()
  );
  mqttClient.publish(MQTT_TOPIC, connected);
  led_toggle();

  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"IP\":\"%s\"}",
    id,
    espClient.localIP().toString().c_str()
  );
  mqttClient.publish(MQTT_TOPIC,connected);

  led_toggle();
  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"FreeHeap\":\"%ld\"}",
    id,
    ESP.getFreeHeap()
  );
  mqttClient.publish(MQTT_TOPIC,connected);

  led_toggle();
  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"BootMode\":\"%ld\"}",
    id,
    ESP.getBootMode()
  );
  mqttClient.publish(MQTT_TOPIC,connected);

  led_toggle();
  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"BootVersion\":\"%ld\"}",
    id,
    ESP.getBootVersion()
  );
  mqttClient.publish(MQTT_TOPIC,connected);

  led_toggle();
  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"FlashChipId\":\"%ld\"}",
    id,
    ESP.getFlashChipId()
  );
  mqttClient.publish(MQTT_TOPIC,connected);

  led_toggle();
  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"FlashChipSize\":\"%ld\"}",
    id,
    ESP.getFlashChipRealSize()
  );
  mqttClient.publish(MQTT_TOPIC,connected);

  led_set(false);
}

#endif // OSX
#endif // UNIT_TEST
