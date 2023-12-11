#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ETH.h>
#include <HTTPClient.h>
#include <AsyncMqttClient.h>

void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void onMqttPublish(uint16_t packetId);
void publishToMQTT(const char* topic, const char* message);
void publishToMQTT(const char* topic, int message);
void publishToMQTT(const char* topic, float message);
void publishToMQTT(const char* topic, bool message);
void initMQTT();

AsyncMqttClient& getMqttClient();
AsyncMqttClientDisconnectReason& getMqttDisconnectReason();
bool getMqttConnected();
bool getMqttEnabled();
bool configureMqtt(
  bool enabled,
  String host,
  uint16_t port,
  String username,
  String password, 
  String clientId,
  uint16_t keepAlive,
  bool cleanSession,
  uint16_t maxTopicLength
);
void getDefaultMqttConf(JsonVariant& root);
bool configureMqtt(JsonVariant& root);
void reconnectMQTTIfNeeded();