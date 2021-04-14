#ifndef MQTT_H
#define MQTT_H

#define QOS             1
#define TIMEOUT         2000L
#define KEEP_ALIVE      20
#define MAX_MSG_SIZE    300

#include "ICommunication.h"
#include "MQTTClient.h"

typedef struct mqttMessageStruct {
    char* topic;
    char* payload;
} MqttStruct;

class Mqtt : public ICommunication
{
private:
    MqttStruct mqttMessage;
    void Subscribe(const char* topic);
    void Publish(const char* topic, char* payload);

    static void connlost(void *context, char *cause);
    static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);

public:
    Mqtt(/* args */);
    ~Mqtt();

    void Setup();
    char* Receive();
    void Send(const char* topic, char* payload);
    void Disconnect();
};



#endif /* MQTT_H */