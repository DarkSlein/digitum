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

#define FACTORY_OUTPUT_TOPIC_PATH "/digitum/intercom_bridge4823/out/"
#define JSON_TOPIC_PATH "/digitum/intercom_bridge4823/out/json"

#define MAC_ADDRESS_MQTT_TOPIC "mac"
#define IP_ADDRESS_MQTT_TOPIC "ip"
#define SERIAL_NUMBER_MQTT_TOPIC "sn"

#define FLAT_NUMBER_MQTT_TOPIC "flat_number"
#define STATE_MQTT_TOPIC "state"

#define INTERCOM_JOURNAL_FLATS_NUMBER 100

#define LED_PIN 32
#define DRY_CONT_PIN 15
#define DOOR_SENS_PIN 114

#define DATA_PIN 12
#define DATA_PERIOD 1 // microseconds

#define PRINT_RAW_SIGNAL_FLAG 0
#define PRINT_MQTT_DEBUG_FLAG 0

#define MAX_FEATURES_SIZE 256
#define MAX_NETWORK_STATUS_SIZE 1024
#define MAX_NETWORK_SETTINGS_SIZE 1024
#define MAX_INTERCOM_STATUS_SIZE 1024
#define MAX_INTERCOM_JOURNAL_SIZE 1024
#define MAX_INTERCOM_SETTINGS_SIZE 1024
#define MAX_INTERCOM_SWITCH_DOOR_SIZE 1024
#define MAX_MQTT_STATUS_SIZE 1024
#define MAX_MQTT_SETTINGS_SIZE 1024
#define MAX_ESP_STATUS_SIZE 1024
#define MAX_LOG_IN_SIZE 1024

#define FS_CONFIG_DIRECTORY "/config"
#define NETWORK_SETTINGS_PATH "/config/networkSettings.json"
#define INTERCOM_SETTINGS_PATH "/config/intercomSettings.json"
#define MQTT_SETTINGS_PATH "/config/mqttSettings.json"
#define USERS_PATH "/config/users.json"

#define JWT_SECRET_KEY "secret"
#define FACTORY_ADMIN_USERNAME "admin"
#define FACTORY_ADMIN_PASSWORD "admin"

#define FACTORY_KMN_MODEL "Vizit"
#define FACTORY_FIRST_APARTMENT 1