#ifndef MQTT_GAME_H
#define MQTT_GAME_H

// Function to initialize MQTT connection
void mqtt_init(const char* client_id);

// Function to publish a message to a topic
void mqtt_publish(const char* topic, const char* message);

// Function to subscribe to a topic
void mqtt_subscribe(const char* topic);

// Function to receive a message from a topic (blocking or with timeout)
void mqtt_receive(char* buffer, int buffer_len);

#endif // MQTT_GAME_H