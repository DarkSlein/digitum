#pragma once

#define MQTT_HOST IPAddress(192, 168, 1, 173)
#define MQTT_PORT 1883

#define SERIAL_NUMBER "4823"

#define MAC_ADDRESS_MQTT_TOPIC "/digitum/intercom_bridge4823/out/mac"
#define IP_ADDRESS_MQTT_TOPIC "/digitum/intercom_bridge4823/out/ip"
#define SERIAL_NUMBER_MQTT_TOPIC "/digitum/intercom_bridge4823/out/sn"

#define FLAT_NUMBER_MQTT_TOPIC "/digitum/intercom_bridge4823/out/flat_number"
#define STATE_MQTT_TOPIC "/digitum/intercom_bridge4823/out/state"

#define LED_PIN 32
#define DRY_CONT_PIN 15
#define DOOR_SENS_PIN 114

#define DATA_PIN 12
#define DATA_PERIOD 240 // microseconds

#define PRINT_RAW_SIGNAL_FLAG 0