#include <stdio.h>
#include <string.h>
#include <mosquitto.h>
#include "mqtt_game.h"

struct mosquitto *mosq;

void mqtt_setup() {
    mosquitto_lib_init();
    mosq = mosquitto_new("client-id", true, NULL);
    mosquitto_connect(mosq, "localhost", 1883, 60);
    mosquitto_loop_start(mosq);
}

void publish_move(const char *msg) {
    mosquitto_publish(mosq, NULL, "/game/move", strlen(msg), msg, 0, false);
}

void receive_move(char *buf) {
    mosquitto_subscribe(mosq, NULL, "/game/move", 0);
    // Blocking receive for simplicity
    struct mosquitto_message *msg;
    while (1) {
        mosquitto_loop(mosq, -1, 1);
        if (mosquitto_message_copy(&msg, NULL) == MOSQ_ERR_SUCCESS) {
            strcpy(buf, msg->payload);
            break;
        }
    }
}