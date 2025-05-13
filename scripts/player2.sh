#!/bin/bash

BROKER="localhost"  # Replace with actual broker IP/host
TOPIC="tictactoe/move"

while true; do
    SLEEP=$((RANDOM % 3 + 1))
    sleep $SLEEP
    MOVE=$((RANDOM % 9 + 1))
    mosquitto_pub -h $BROKER -t $TOPIC -m "$MOVE"
done