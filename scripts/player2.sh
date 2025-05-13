#!/bin/bash

TOPIC="/game/move"
BROKER="localhost"

while true; do
  msg=$(mosquitto_sub -h $BROKER -t $TOPIC -C 1)
  echo "Received: $msg"
  move=$((RANDOM % 9 + 1))
  mosquitto_pub -h $BROKER -t $TOPIC -m "$move"
  echo "Bash sent move $move"
done