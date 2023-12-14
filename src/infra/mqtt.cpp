#include "config/config.h"
#include "infra/mqtt.h"
#include "infra/eth.h"
#include "infra/fs.h"
#include "utils/settings.h"

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
AsyncMqttClientDisconnectReason mqttDisconnectReason;

DynamicJsonDocument mqttConf(1024);
String mqttOutputJson = "";
bool mqttConnected = false;
bool mqttEnabled = true;

// Pointers to hold retained copies of the mqtt client connection strings.
// This is required as AsyncMqttClient holds refrences to the supplied connection strings.
char* retainedHost;
char* retainedClientId;
char* retainedUsername;
char* retainedPassword;

template <typename T>
void publishJSONToMQTT(const char* topic, T message) {
  DynamicJsonDocument doc(8192);
  DeserializationError error = deserializeJson(doc, mqttOutputJson);

  doc[topic] = message;

  mqttOutputJson = "";
  serializeJson(doc, mqttOutputJson);

  if (PRINT_MQTT_DEBUG_FLAG)
    println("Sending to MQTT topic '", JSON_TOPIC_PATH, "': ", mqttOutputJson);
  mqttClient.publish(JSON_TOPIC_PATH, 2, true, mqttOutputJson.c_str());
}

const char* getFullTopic(const char* topic) {
  return (String(DEFAULT_OUTPUT_TOPIC_PATH) + String(topic)).c_str();
}

void publishToMQTT(const char* topic, const char* message) {
  const char* fullTopic = getFullTopic(topic);
 
  if (PRINT_MQTT_DEBUG_FLAG)
    println("Sending to MQTT topic '", fullTopic, "': ", message);

  mqttClient.publish(fullTopic, 2, true, message);
  publishJSONToMQTT(topic, message);
}

void publishToMQTT(const char* topic, int message) {
  const char* fullTopic = getFullTopic(topic);
 
  if (PRINT_MQTT_DEBUG_FLAG)
    println("Sending to MQTT topic '", fullTopic, "': ", message);

  char num_char[10];
  sprintf(num_char, "%d", message);
  mqttClient.publish(fullTopic, 2, true, num_char);

  publishJSONToMQTT(topic, message);
}

void publishToMQTT(const char* topic, float message) {
  const char* fullTopic = getFullTopic(topic);
 
  if (PRINT_MQTT_DEBUG_FLAG)
    println("Sending to MQTT topic '", fullTopic, "': ", message);

  char num_char[10];
  dtostrf(message, 1, 2, num_char); // Convert float to string
  mqttClient.publish(fullTopic, 2, true, num_char);

  publishJSONToMQTT(topic, message);
}

void publishToMQTT(const char* topic, bool message) {
  const char* fullTopic = getFullTopic(topic);
 
  if (PRINT_MQTT_DEBUG_FLAG)
    println("Sending to MQTT topic '", fullTopic, "': ", message);

  const char* bool_str = message ? "true" : "false";
  mqttClient.publish(topic, 2, true, bool_str);

  publishJSONToMQTT(topic, message);
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);

  mqttConnected = true;

  publishToMQTT(MAC_ADDRESS_MQTT_TOPIC, ETH.macAddress().c_str());
  publishToMQTT(IP_ADDRESS_MQTT_TOPIC, ETH.localIP().toString().c_str());
  publishToMQTT(SERIAL_NUMBER_MQTT_TOPIC, SERIAL_NUMBER);

  //uint16_t packetIdSub = mqttClient.subscribe("esp32/led", 0);
  //Serial.print("Subscribing at QoS 0, packetId: ");
  //Serial.println(packetIdSub);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  mqttConnected = false;
  mqttDisconnectReason = reason;
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

AsyncMqttClient& getMqttClient() {
  return mqttClient;
}

AsyncMqttClientDisconnectReason& getMqttDisconnectReason() {
  return mqttDisconnectReason;
}

bool getMqttConnected() {
  return mqttConnected;
}

bool getMqttEnabled() {
  return mqttEnabled;
}

/**
 * Retains a copy of the cstr provided in the pointer provided using dynamic allocation.
 *
 * Frees the pointer before allocation and leaves it as nullptr if cstr == nullptr.
 */
static char* retainCstr(const char* cstr, char** ptr) {
  // free up previously retained value if exists
  free(*ptr);
  *ptr = nullptr;

  // dynamically allocate and copy cstr (if non null)
  if (cstr != nullptr) {
    *ptr = (char*)malloc(strlen(cstr) + 1);
    strcpy(*ptr, cstr);
  }

  // return reference to pointer for convenience
  return *ptr;
}

void setMqttServer(String host, uint16_t port) {
  Serial.print("MQTT server: ");
  Serial.print(host);
  Serial.print(":");
  Serial.println(port);
  mqttClient.setServer(retainCstr(host.c_str(), &retainedHost), port);
}

bool checkMqttHost(String host, uint16_t port) {
  if (!checkHost(host.c_str(), port)) {
    Serial.print("MQTT host is not responding: ");
    Serial.print(host);
    Serial.print(":");
    Serial.println(port);
    return false;
  }
  return true;
}

bool configureMqtt(bool enabled, String host, uint16_t port, String username, String password, 
String clientId, uint16_t keepAlive, bool cleanSession, uint16_t maxTopicLength) {
  if (!mqttEnabled) {
    mqttEnabled = enabled;
    return checkMqttHost(host, port);
  }

  mqttEnabled = enabled;
  mqttClient.disconnect();

  if (!checkMqttHost(host, port))
    return false;

  if (enabled) {
    Serial.println(F("Reconnecting to MQTT..."));
    setMqttServer(host, port);
    if (username.length() > 0) {
      mqttClient.setCredentials(
          retainCstr(username.c_str(), &retainedUsername),
          retainCstr(password.length() > 0 ? password.c_str() : nullptr, &retainedPassword));
    } else {
      mqttClient.setCredentials(retainCstr(nullptr, &retainedUsername), retainCstr(nullptr, &retainedPassword));
    }
    mqttClient.setClientId(retainCstr(clientId.c_str(), &retainedClientId));
    mqttClient.setKeepAlive(keepAlive);
    mqttClient.setCleanSession(cleanSession);
    mqttClient.setMaxTopicLength(maxTopicLength);
    mqttClient.connect();
    return true;
  }

  return false;
}

bool configureMqtt(JsonVariant& root) {
  configureMqtt(
    root["enabled"].as<bool>(),
    root["host"].as<String>(),
    root["port"].as<uint16_t>(),
    root["username"].as<String>(),
    root["password"].as<String>(),
    root["client_id"].as<String>(),
    root["keep_alive"].as<uint16_t>(),
    root["clean_session"].as<bool>(),
    root["max_topic_length"].as<uint16_t>()
  );
}

void getDefaultMqttConf(JsonVariant& root) {
  root["enabled"] = FACTORY_MQTT_ENABLED;
  root["host"] = FACTORY_MQTT_HOST;
  root["port"] = FACTORY_MQTT_PORT;
  root["username"] = FACTORY_MQTT_USERNAME;
  root["password"] = FACTORY_MQTT_PASSWORD;
  root["client_id"] = formatSetting(FACTORY_MQTT_CLIENT_ID);
  root["keep_alive"] = FACTORY_MQTT_KEEP_ALIVE;
  root["clean_session"] = FACTORY_MQTT_CLEAN_SESSION;
  root["max_topic_length"] = FACTORY_MQTT_MAX_TOPIC_LENGTH;
}

bool loadMqttConfig() {
  bool success = readJsonVariantFromFile(MQTT_SETTINGS_PATH, mqttConf);

  if (!success)
    return false;
}

void reconnectMQTTIfNeeded() {
  if (!mqttClient.connected()) {
    if (mqttConf.isNull()) 
      loadMqttConfig();

    JsonVariant root = mqttConf.as<JsonVariant>();

    if (!mqttConf.is<JsonVariant>())
      return;

    bool mqttConfigured = configureMqtt(
      root["enabled"].as<bool>(),
      root["host"].as<String>(),
      root["port"].as<uint16_t>(),
      root["username"].as<String>(),
      root["password"].as<String>(),
      root["client_id"].as<String>(),
      root["keep_alive"].as<uint16_t>(),
      root["clean_session"].as<bool>(),
      root["max_topic_length"].as<uint16_t>()
   );
  }
}

void initMQTT() {
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  //mqttClient.onMessage(onMqttMessage);
  //mqttClient.onPublish(onMqttPublish);
}
