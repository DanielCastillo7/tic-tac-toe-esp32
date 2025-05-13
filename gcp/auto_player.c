#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "Auto_Player_C"
#define TOPIC       "tictactoe/move"
#define QOS         1
#define TIMEOUT     10000L

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect to broker.\n");
        exit(1);
    }

    srand(time(NULL));
    while (1) {
        int move = rand() % 9 + 1;
        char message[2];
        snprintf(message, sizeof(message), "%d", move);

        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        pubmsg.payload = message;
        pubmsg.payloadlen = strlen(message);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_deliveryToken token;
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        MQTTClient_waitForCompletion(client, token, TIMEOUT);

        sleep(2); // Wait before sending next move
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}