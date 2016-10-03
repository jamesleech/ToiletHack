#ifndef UNIT_TEST
#ifndef OSX

#ifndef __BRIDGE_MQTT_CLIENT_H
#define __BRIDGE_MQTT_CLIENT_H

#include <PubSubClient.h>
#include <status.h>
#include "../user_config.h"

class BridgeMQTTClient
{
  public:
    BridgeMQTTClient(unsigned int bridgeId, PubSubClient* pubSubClient); //TODO: How to we mock this sucker in c++
    void Publish(const char* payload);
    void Handle();
    void PublishStatus(Status* status);

  private:
    const char* mqtt_server = network_mqtt_server; //The IP of the mqtt server

    unsigned int _bridgeId;
    PubSubClient* _pubSubClient;
    bool guardConnect();
    bool reconnect();
    void sub_callback(char* topic, byte* payload, unsigned int length);
    void publishConnected(); //called after connecting to the server

    void led_toggle();
    void led_set(bool on);
};

#endif // __BRIDGE_MQTT_CLIENT_H
#endif // OSX
#endif // UNIT_TEST
