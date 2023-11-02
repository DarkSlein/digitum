#pragma once

#include <HTTPClient.h>
#include <AsyncMqttClient.h>

void connectToMqtt();
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void onMqttPublish(uint16_t packetId);
void sendDataToMQTT(int flatNumber);
void publishToMQTT(const char* topic, const char* message);
void publishToMQTT(const char* topic, int message);
void publishToMQTT(const char* topic, float message);
void publishToMQTT(const char* topic, bool message);
void initMQTT();