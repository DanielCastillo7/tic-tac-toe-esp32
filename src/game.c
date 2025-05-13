#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "Mac_TUI"
#define TOPIC_BOARD "tictactoe/board"
#define QOS         1
#define TIMEOUT     10000L

void messageArrived(char *topicName, int topicLen, MQTTClient_message *message) {
    printf("\n--- Game Board ---\n");
    printf("%.*s\n", message->payloadlen, (char*)message->payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
}

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect to broker.\n");
        exit(1);
    }

    MQTTClient_subscribe(client, TOPIC_BOARD, QOS);

    printf("Waiting for game updates...\n");
    while (1) {
        sleep(1); // Loop keeps the client active
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}