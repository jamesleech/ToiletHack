#ifndef UNIT_TEST
#ifndef OSX

#include <functional>
#include "bridgeMQTTClient.h"

#define MQTT_TOPIC "ToiletHack"

using namespace std::placeholders;

long mqtt_lastReconnectAttempt = 0;

BridgeMQTTClient::BridgeMQTTClient(PubSubClient* pubSubClient) {
  _pubSubClient = pubSubClient;

  _pubSubClient->setServer(mqtt_server, 1883);
  _pubSubClient->setCallback(
    std::bind(&BridgeMQTTClient::sub_callback, this, _1, _2, _3)
  );
}

void BridgeMQTTClient::Publish(const char* payload) {
  _pubSubClient->publish(MQTT_TOPIC, payload);
}

void BridgeMQTTClient::PublishStatus(Status* status) {

  char* status_json = status->toJson();
  Publish(status_json);
}

void BridgeMQTTClient::Handle() {
  //mqtt connected?
  if (!_pubSubClient->connected()) {
    long now = millis();
    //only try and reconnect every 5 seconds
    if (now - mqtt_lastReconnectAttempt > 5000) {
      mqtt_lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        mqtt_lastReconnectAttempt = 0;
      }
    }
  }

  if (_pubSubClient->connected()) {
    //do mqtt subscribe
    _pubSubClient->loop();
  }
}

bool BridgeMQTTClient::reconnect() {

  if (_pubSubClient->connect("ToiletHackNode0")) {
    publishConnected(); // Once connected, publish an announcement...

    char mqttchannel[30];
    snprintf(mqttchannel, 30, "ToiletHack.Node.%ld", ESP.getChipId());

    _pubSubClient->subscribe(mqttchannel, 1);
  }
  return _pubSubClient->connected();
}

void BridgeMQTTClient::sub_callback(char* topic, byte* payload, unsigned int length) {
  char mqtt_received[length];

  //assume the bytes in the payload are chars
  for (int i = 0; i < length; i++) {
    mqtt_received[i] = (char)payload[i];
  }

  Publish("Got it!");
}

void BridgeMQTTClient::publishConnected() {
  led_set(true);
  char connected[50];
  uint32_t id = ESP.getChipId();

  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"Vcc\":\"%ld\"}",
    id,
    ESP.getVcc()
  );
  Publish(connected);
  led_toggle();

  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"FreeHeap\":\"%ld\"}",
    id,
    ESP.getFreeHeap()
  );
  Publish(connected);
  led_toggle();

  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"BootMode\":\"%ld\"}",
    id,
    ESP.getBootMode()
  );
  Publish(connected);
  led_toggle();

  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"BootVersion\":\"%ld\"}",
    id,
    ESP.getBootVersion()
  );
  Publish(connected);
  led_toggle();

  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"FlashChipId\":\"%ld\"}",
    id,
    ESP.getFlashChipId()
  );
  Publish(connected);
  led_toggle();

  snprintf(connected, 50,
    "{\"Node\":\"%ld\",\"FlashChipSize\":\"%ld\"}",
    id,
    ESP.getFlashChipRealSize()
  );
  Publish(connected);

  led_set(false);
}

void BridgeMQTTClient::led_toggle() {}
void BridgeMQTTClient::led_set(bool on) {}

#endif // OSX
#endif // UNIT_TEST
