#include <Arduino.h>
#include <ETH.h>

#include "config/config.h"
#include "infra/mqtt.h"

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;

void connectToMqtt() {
    Serial.println("Connecting to MQTT...");
    mqttClient.connect();
}

void publishToMQTT(const char* topic, const char* message) {
  mqttClient.publish(topic, 2, true, message);
}

void publishToMQTT(const char* topic, int message) {
  char num_char[10];
  sprintf(num_char, "%d", message);
  mqttClient.publish(topic, 2, true, num_char);
}

void publishToMQTT(const char* topic, float message) {
  char num_char[10];
  dtostrf(message, 1, 2, num_char); // Convert float to string
  mqttClient.publish(topic, 2, true, num_char);
}

void publishToMQTT(const char* topic, bool message) {
  const char* bool_str = message ? "true" : "false";
  mqttClient.publish(topic, 2, true, bool_str);
}

void onMqttConnect(bool sessionPresent) {
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);

    publishToMQTT(MAC_ADDRESS_MQTT_TOPIC, ETH.macAddress().c_str());
    publishToMQTT(IP_ADDRESS_MQTT_TOPIC, ETH.localIP().toString().c_str());
    publishToMQTT(SERIAL_NUMBER_MQTT_TOPIC, SERIAL_NUMBER);

    //uint16_t packetIdSub = mqttClient.subscribe("esp32/led", 0);
    //Serial.print("Subscribing at QoS 0, packetId: ");
    //Serial.println(packetIdSub);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void initMQTT() {
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  //mqttClient.onMessage(onMqttMessage);
  //mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
}