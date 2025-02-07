#include "app/routes.h"
#include "config/config.h"

#include "utils/print.h"
#include "utils/settings.h"
#include "utils/utils.h"
#include "utils/errorCodes.h"

#include "infra/eth.h"
#include "infra/httpServer.h"
#include "infra/mqtt.h"
#include "infra/relay.h"
#include "infra/fs.h"
#include "infra/intercom.h"

#include "domain/services/intercomJournal.h"
#include "domain/services/auth.h"

void handleDoorOpen(AsyncWebServerRequest *request) {
  relayTurnOn();
  delay(2000);
  relayTurnOff();
  println("Door is open by ", request->client()->remoteIP().toString());

  request->send(200, "text/plain", "OK");
}

void getFeatures(AsyncWebServerRequest *request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_FEATURES_SIZE);
  JsonObject root = response->getRoot();

  root["project"] = false;
  root["security"] = true;
  root["mqtt"] = true;
  root["ntp"] = false;
  root["ota"] = false;
  root["upload_firmware"] = true;

  response->setLength();
  request->send(response);
}

void intercomStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_INTERCOM_STATUS_SIZE);
  JsonObject root = response->getRoot();

  root["status"] = (int) getIntercomStatus();
  root["lastCalledNumber"] = getLastCalledNumber();
  root["doorStatus"] = false;

  response->setLength();
  request->send(response);
}

static void intercomJournalRead(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_INTERCOM_JOURNAL_SIZE);
  JsonObject root = response->getRoot();

  const size_t CAPACITY = JSON_ARRAY_SIZE(5*INTERCOM_JOURNAL_FLATS_NUMBER);
  StaticJsonDocument<CAPACITY> flatsDoc;

  JsonArray flats = flatsDoc.to<JsonArray>();
  getIntercomJournalAsJson(flats);

  root["notes"] = flats;

  response->setLength();
  request->send(response);
}

static void intercomSettingsRead(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_INTERCOM_SETTINGS_SIZE);
  DynamicJsonDocument doc(1024);

  bool success = readJsonVariantFromFile(INTERCOM_SETTINGS_PATH, doc);
  JsonVariant root = doc.as<JsonVariant>();

  if (!success)
    getDefaultIntercomConf(root);

  String jsonString;
  serializeJson(root, jsonString);
  request->send(200, "application/json", jsonString.c_str());
}

static void intercomSettingsUpdate(AsyncWebServerRequest *request, 
uint8_t *data, size_t len, size_t index, size_t total) {

  DynamicJsonDocument jsonDoc(MAX_INTERCOM_SETTINGS_SIZE);
  String jsonStr = requestDataToStr(data, len);
  DeserializationError error = deserializeJson(jsonDoc, jsonStr);

  if (!jsonDoc.is<JsonVariant>()) {
    request->send(400);
    return;
  }

  JsonVariant root = jsonDoc.as<JsonVariant>();

  bool success = writeJsonToFile(INTERCOM_SETTINGS_PATH, root);

  if (success) {
    String jsonString;
    serializeJson(root, jsonString);
    request->send(200, "application/json", jsonString.c_str());
  }
  else
    request->send(500, "text/plain", "Intercom settings not updated");

  configureIntercom(
    root["kmnModel"],
    root["firstApartment"]
  );
}

static void switchDoor(AsyncWebServerRequest *request, 
uint8_t *data, size_t len, size_t index, size_t total) {
  DynamicJsonDocument jsonDoc(MAX_INTERCOM_SWITCH_DOOR_SIZE);
  String jsonStr = requestDataToStr(data, len);
  DeserializationError error = deserializeJson(jsonDoc, jsonStr);

  if (!jsonDoc.is<JsonVariant>()) {
    request->send(400);
    return;
  }

  JsonVariant root = jsonDoc.as<JsonVariant>();

  SwitchDoorType switchDoorType = static_cast<SwitchDoorType>(root["type"].as<int>());
  DoorStatus doorStatus = static_cast<DoorStatus>(root["status"].as<int>());

  switchRelay(
    switchDoorType,
    doorStatus,
    root["time"]
  );

  request->send(200, "application/json");
}

void networkStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_NETWORK_STATUS_SIZE);
  JsonObject root = response->getRoot();

  getEthStatus(root);

  response->setLength();
  request->send(response);
}

void scanNetworks(AsyncWebServerRequest* request) {
  if (WiFi.scanComplete() != -1) {
    WiFi.scanDelete();
    WiFi.scanNetworks(true);
  }
  request->send(202);
}

static void networkSettingsRead(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_NETWORK_SETTINGS_SIZE);
  DynamicJsonDocument doc(1024);

  bool success = readJsonVariantFromFile(NETWORK_SETTINGS_PATH, doc);
  JsonVariant root = doc.as<JsonVariant>();

  if (!success)
    getDefaultEthConf(root);

  response->setLength();

  String jsonString;
  serializeJson(root, jsonString);
  request->send(200, "application/json", jsonString.c_str());
}

static void networkSettingsUpdate(AsyncWebServerRequest *request, 
uint8_t *data, size_t len, size_t index, size_t total) {

  DynamicJsonDocument jsonDoc(MAX_NETWORK_SETTINGS_SIZE);
  String jsonStr = requestDataToStr(data, len);
  DeserializationError error = deserializeJson(jsonDoc, jsonStr);

  if (!jsonDoc.is<JsonVariant>()) {
    request->send(400);
    return;
  }

  JsonVariant root = jsonDoc.as<JsonVariant>();

  bool success = writeJsonToFile(NETWORK_SETTINGS_PATH, root);

  if (success) {
    String jsonString;
    serializeJson(root, jsonString);
    request->send(200, "application/json", jsonString.c_str());
  }
  else
    request->send(500, "text/plain", "Network settings not updated");

  configureEth(
    root["static_ip_config"].as<bool>(),
    root["local_ip"].as<String>(),
    root["gateway_ip"].as<String>(),
    root["subnet_mask"].as<String>(),
    root["dns_ip"].as<String>()
  );
}

static void mqttStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_MQTT_STATUS_SIZE);
  JsonObject root = response->getRoot();

  root["enabled"] = getMqttEnabled();
  root["connected"] = getMqttClient().connected();
  root["client_id"] = getMqttClient().getClientId();
  root["disconnect_reason"] = (uint8_t)getMqttDisconnectReason();

  response->setLength();
  request->send(response);
}

static void mqttSettingsRead(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_MQTT_SETTINGS_SIZE);
  DynamicJsonDocument doc(1024);

  bool success = readJsonVariantFromFile(MQTT_SETTINGS_PATH, doc);
  JsonVariant root = doc.as<JsonVariant>();

  if (!success)
    getDefaultMqttConf(root);

  response->setLength();

  String jsonString;
  serializeJson(root, jsonString);
  request->send(200, "application/json", jsonString.c_str());
}

static void mqttSettingsUpdate(AsyncWebServerRequest *request, 
uint8_t *data, size_t len, size_t index, size_t total) {

  DynamicJsonDocument jsonDoc(MAX_MQTT_SETTINGS_SIZE);
  String jsonStr = requestDataToStr(data, len);
  DeserializationError error = deserializeJson(jsonDoc, jsonStr);

  if (!jsonDoc.is<JsonVariant>() || error) {
    request->send(400);
    return;
  }

  JsonVariant root = jsonDoc.as<JsonVariant>();

  bool fileLoaded = writeJsonToFile(MQTT_SETTINGS_PATH, root);
  bool enabled = root["enabled"].as<bool>();

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

  if (!fileLoaded) {
    request->send(500, "text/plain", "MQTT settings not updated");
  } else if (!mqttConfigured && enabled) {
    request->send(404, "text/plain", "MQTT settings not configured");
  } else {
    String jsonString;
    serializeJson(root, jsonString);
    request->send(200, "application/json", jsonString.c_str());
  }
}

void systemStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_ESP_STATUS_SIZE);
  JsonObject root = response->getRoot();

  root["esp_platform"] = "esp32";
  root["max_alloc_heap"] = ESP.getMaxAllocHeap();
  root["psram_size"] = ESP.getPsramSize();
  root["free_psram"] = ESP.getFreePsram();  
  root["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
  root["free_heap"] = ESP.getFreeHeap();
  root["sketch_size"] = ESP.getSketchSize();
  root["free_sketch_space"] = ESP.getFreeSketchSpace();
  root["sdk_version"] = ESP.getSdkVersion();
  root["flash_chip_size"] = ESP.getFlashChipSize();
  root["flash_chip_speed"] = ESP.getFlashChipSpeed();

  root["fs_total"] = LittleFS.totalBytes();
  root["fs_used"] = LittleFS.usedBytes();

  response->setLength();
  request->send(response);
}

void handleDoorStatus(AsyncWebServerRequest *request) {

  request->send(200, "text/plain", "OK");
}


void handleBodyRequest(AsyncWebServer& server, const char* uri, WebRequestMethodComposite method,
ArJsonRequestHandlerFunction onRequest) {
  AsyncCallbackJsonWebHandler* handler = \
    new AsyncCallbackJsonWebHandler(uri, onRequest);
  handler->setMethod(method);
  server.addHandler(handler);
}

void restartNow(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "OK");
  ESP.restart();
}

void factoryReset(AsyncWebServerRequest *request) {
  deleteFilesInDir(FS_CONFIG_DIRECTORY);
  restartNow(request);
}

void verifyAuthorization(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "OK");
}

static void signIn(AsyncWebServerRequest *request, 
uint8_t *data, size_t len, size_t index, size_t total) {

  DynamicJsonDocument jsonDoc(MAX_LOG_IN_SIZE);
  String jsonStr = requestDataToStr(data, len);
  println("Access token: ", jsonStr);
  DeserializationError error = deserializeJson(jsonDoc, jsonStr);

  if (!jsonDoc.is<JsonVariant>() || error) {
    request->send(400);
    return;
  }

  JsonVariant root = jsonDoc.as<JsonVariant>();

  String accessToken;
  StatusCode status = getAccessToken(
    root["username"].as<String>(), 
    root["password"].as<String>(),
    accessToken
  );
  println("Access token: ", accessToken);

  if (status == StatusCode::NOT_FOUND) {
    request->send(403, "text/plain", "Authorization failed");
  }

  if (!accessToken) {
    request->send(500, "text/plain", "Access token not generated");
  } else {
    DynamicJsonDocument jsonDoc(MAX_LOG_IN_SIZE);
    JsonVariant res = jsonDoc.as<JsonVariant>();

    res["access_token"] = accessToken;

    String jsonString;
    serializeJson(res, jsonString);
    println("Sign in response: ", jsonString);

    request->send(200, "application/json", jsonString.c_str());
  }
}

void initRoutes() {
  AsyncWebServer& server = getServer();
  server.on("/api/v1/door/open", handleDoorOpen);
  server.on("/api/v1/door/status", handleDoorStatus);

  server.on("/api/v1/features", getFeatures);

  server.on("/api/v1/networkStatus", networkStatus);
  server.on("/api/v1/networkSettings", HTTP_GET, networkSettingsRead);
  server.on("/api/v1/networkSettings", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, networkSettingsUpdate);

  server.on("/api/v1/intercomStatus", intercomStatus);
  server.on("/api/v1/intercomJournal", intercomJournalRead);
  server.on("/api/v1/intercomSettings", HTTP_GET, intercomSettingsRead);
  server.on("/api/v1/intercomSettings", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, intercomSettingsUpdate);
  server.on("/api/v1/switchDoor", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, switchDoor);

  server.on("/api/v1/mqttStatus", mqttStatus);
  server.on("/api/v1/mqttSettings", HTTP_GET, mqttSettingsRead);
  server.on("/api/v1/mqttSettings", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, mqttSettingsUpdate);

  server.on("/api/v1/systemStatus", systemStatus);
  server.on("/api/v1/restart", restartNow);
  server.on("/api/v1/factoryReset", factoryReset);

  server.on("/api/v1/verifyAuthorization", HTTP_GET, verifyAuthorization);
  server.on("/api/v1/signIn", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, signIn);
}