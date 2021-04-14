extern "C"{
    #include <stdlib.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <string.h>
    #include "MQTTClient.h"
}

#include <functional>
#include "Mqtt.h"
#include "credentials.h"

MQTTClient client;
// Mqtt MqttClass;

Mqtt::Mqtt(/* args */)
{

}

Mqtt::~Mqtt()
{
    
}

/* Subscribed MQTT topic listener function. */
int Mqtt::msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    if(message) {       
        ((Mqtt*) context)->mqttMessage.topic = topicName;
        ((Mqtt*) context)->mqttMessage.payload = (char*)message->payload;
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}

void Mqtt::connlost(void *context, char *cause)
{
    printf("Connection lost\n");
    if (cause)
        printf("Reason is : %s\n", cause);
}

void Mqtt::Subscribe(const char* topic)
{
    printf("Subscribing to topic %s for client %s using QoS%d\n\n", 
        topic, CLIENTID, QOS);

    if (topic != NULL)
    {
        MQTTClient_subscribe(client, topic, QOS);
    }
}

void Mqtt::Publish(const char* topic, char* message)
{
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    pubmsg.payload = message;
    pubmsg.payloadlen = (int)strlen(message);
    pubmsg.qos = QOS;
    pubmsg.retained = 1;
    
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    int rc = MQTTClient_waitForCompletion(client, token, 1000);
    rc = rc;
}

void Mqtt::Disconnect()
{
    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);
}

void Mqtt::Setup()
{    
    int rc = -1;
    printf("Initializing MQTT...\n");
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = KEEP_ALIVE;
    conn_opts.cleansession = 1;
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;
    MQTTClient_create(&client, BROKER_ADDR, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);

    MQTTClient_setCallbacks(client, this, connlost, msgarrvd, NULL);

    while ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        sleep(TIMEOUT / 1000); 
    }

    Subscribe(TOPIC_WAREHOUSE);
}

void Mqtt::Send(const char* topic, char* payload)
{
    Publish(topic, payload);
}

char* Mqtt::Receive()
{
    char* tempMsg;
    
    tempMsg = this->mqttMessage.payload;
    mqttMessage.payload = nullptr;
    return tempMsg;
}