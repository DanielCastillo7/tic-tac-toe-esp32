#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
    printf("Received message: %s\n", (char *)msg->payload);
    int move = rand() % 9 + 1;
    char buf[2];
    sprintf(buf, "%d", move);
    mosquitto_publish(mosq, NULL, "/game/move", strlen(buf), buf, 0, false);
    printf("Sent move: %s\n", buf);
}

int main() {
    mosquitto_lib_init();
    struct mosquitto *mosq = mosquitto_new("auto-player", true, NULL);
    mosquitto_connect(mosq, "localhost", 1883, 60);
    mosquitto_message_callback_set(mosq, on_message);
    mosquitto_subscribe(mosq, NULL, "/game/move", 0);
    mosquitto_loop_forever(mosq, -1, 1);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}