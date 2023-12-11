#pragma once

#define FACTORY_STATIC_LOCAL_IP IPAddress(192, 168, 1, 150)
#define FACTORY_STATIC_GATEWAY IPAddress(192, 168, 1, 1)
#define FACTORY_STATIC_SUBNET IPAddress(255, 255, 255, 0)

#define FACTORY_MQTT_ENABLED false
#define FACTORY_MQTT_HOST ""
#define FACTORY_MQTT_PORT 1883
#define FACTORY_MQTT_USERNAME ""
#define FACTORY_MQTT_PASSWORD ""
#define FACTORY_MQTT_CLIENT_ID "#{platform}-#{unique_id}"
#define FACTORY_MQTT_KEEP_ALIVE 16
#define FACTORY_MQTT_CLEAN_SESSION true
#define FACTORY_MQTT_MAX_TOPIC_LENGTH 128

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
#define DATA_PERIOD 120 // microseconds

#define PRINT_RAW_SIGNAL_FLAG 1

#define MAX_FEATURES_SIZE 256
#define MAX_NETWORK_STATUS_SIZE 1024
#define MAX_NETWORK_SETTINGS_SIZE 1024
#define MAX_INTERCOM_STATUS_SIZE 1024
#define MAX_MQTT_STATUS_SIZE 1024
#define MAX_MQTT_SETTINGS_SIZE 1024
#define MAX_ESP_STATUS_SIZE 1024

#define FS_CONFIG_DIRECTORY "/config"
#define NETWORK_SETTINGS_PATH "/config/networkSettings.json"
#define MQTT_SETTINGS_PATH "/config/mqttSettings.json"