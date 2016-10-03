#ifndef UNIT_TEST
#ifndef OSX

#include "bridgeMQTTClient.h"

#define MQTT_TOPIC "ToiletHack"

using namespace std::placeholders;

long mqtt_lastReconnectAttempt = 0;

BridgeMQTTClient::BridgeMQTTClient(unsigned int bridgeId, PubSubClient* pubSubClient) {
  _bridgeId = bridgeId;
  _pubSubClient = pubSubClient;

  _pubSubClient->setServer(mqtt_server, 1883);
  _pubSubClient->setCallback(
    std::bind(&BridgeMQTTClient::sub_callback, this, _1, _2, _3)
  );
}

void BridgeMQTTClient::Publish(const char* payload) {
  if(guardConnect()) {
    _pubSubClient->publish(MQTT_TOPIC, payload);
  }
}

void BridgeMQTTClient::PublishStatus(Status* status) {
  if(guardConnect()) {
    char* status_json = status->toJson();
    Publish(status_json);
  }
}

void BridgeMQTTClient::Handle() {

  if (guardConnect()) {
    _pubSubClient->loop();
  }

}

bool BridgeMQTTClient::guardConnect() {

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

    return _pubSubClient->connected();
}

bool BridgeMQTTClient::reconnect() {

  char buffer[30];
  snprintf(buffer, 30, "THB_%ld", _bridgeId);

  if (_pubSubClient->connect(buffer)) {
    publishConnected(); // Once connected, publish an announcement...

    snprintf(buffer, 30, "ToiletHack.Node.%ld", _bridgeId);
    _pubSubClient->subscribe(buffer, 1);
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
