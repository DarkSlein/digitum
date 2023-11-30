#include "app/routes.h"
#include "infra/eth.h"
#include "infra/httpServer.h"
#include "infra/relay.h"
#include "infra/fs.h"
#include "config/config.h"
#include "utils/print.h"

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
  root["security"] = false;
  root["mqtt"] = true;
  root["ntp"] = false;
  root["ota"] = false;
  root["upload_firmware"] = true;

  response->setLength();
  request->send(response);
}

void intercomStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_AP_STATUS_SIZE);
  JsonObject root = response->getRoot();

  root["status"] = 0;
  root["ip_address"] = ETH.localIP().toString();
  root["mac_address"] = ETH.macAddress();
  root["station_num"] = WiFi.softAPgetStationNum();

  response->setLength();
  request->send(response);
}

void networkStatus(AsyncWebServerRequest* request) {
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_NETWORK_STATUS_SIZE);
  JsonObject root = response->getRoot();
  wl_status_t status = WL_CONNECTED;

  root["status"] = (uint8_t)status;
  if (status == WL_CONNECTED) {
    root["local_ip"] = ETH.localIP().toString();
    IPv6Address localIPv6 = ETH.localIPv6();
    if (!(localIPv6 == IPv6Address()))
      root["local_ip_v6"] = ETH.localIPv6().toString();
    root["mac_address"] = ETH.macAddress();
    root["full_duplex"] = ETH.fullDuplex();
    root["link_speed"] = ETH.linkSpeed();
    root["link_up"] = ETH.linkUp();
    root["network_id"] = ETH.networkID();
    root["subnet_mask"] = ETH.subnetMask().toString();
    root["gateway_ip"] = ETH.gatewayIP().toString();
    IPAddress dnsIP = ETH.dnsIP();
    if (dnsIP)
      root["dns_ip"] = dnsIP.toString();
  }

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
  AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_NETWORK_STATUS_SIZE);
  DynamicJsonDocument doc(1024);

  bool success = readJsonVariantFromFile(NETWORK_SETTINGS_PATH, doc);
  JsonVariant root = doc.as<JsonVariant>();

  if (!success) {
    root["static_ip_config"] = false;
    root["local_ip"] = ETH.localIP().toString();
    root["gateway_ip"] = ETH.gatewayIP().toString();
    root["subnet_mask"] = ETH.subnetMask().toString();
    root["dns_ip"] = ETH.dnsIP().toString();
    root["dns_ip_2"] = ETH.dnsIP(1).toString();
  }

  response->setLength();

  String jsonString;
  serializeJson(root, jsonString);
  request->send(200, "application/json", jsonString.c_str());
}

static void networkSettingsUpdate(AsyncWebServerRequest* request, JsonVariant &json) {
  if (!json.is<JsonVariant>()) {
    request->send(400);
    return;
  }

  JsonVariant root = json.as<JsonVariant>();

  bool success = writeJsonVariantToFile(NETWORK_SETTINGS_PATH, root);

  setEthConfig(
    root["static_ip_config"].as<bool>(),
    root["local_ip"].as<String>(),
    root["gateway_ip"].as<String>(),
    root["subnet_mask"].as<String>(),
    root["dns_ip"].as<String>()
  );

  if (success) {
    String jsonString;
    serializeJson(root, jsonString);
    request->send(200, "application/json", jsonString.c_str());
  }
  else
    request->send(500, "text/plain", "Network settings not updated");
}

void handleDoorStatus(AsyncWebServerRequest *request) {

  request->send(200, "text/plain", "OK");
}

void handleBodyRequest(AsyncWebServer& server, const char* uri, WebRequestMethodComposite method, \
ArJsonRequestHandlerFunction onRequest) {
  AsyncCallbackJsonWebHandler* handler = \
    new AsyncCallbackJsonWebHandler(uri, networkSettingsUpdate);
  handler->setMethod(method);
  server.addHandler(handler);
}

void initRoutes() {
  AsyncWebServer& server = getServer();
  server.on("/api/v1/door/open", handleDoorOpen);
  server.on("/api/v1/door/status", handleDoorStatus);

  server.on("/api/v1/features", getFeatures);

  server.on("/api/v1/networkStatus", networkStatus);
  server.on("/api/v1/networkSettings", HTTP_GET, networkSettingsRead);
  handleBodyRequest(server, "/api/v1/networkSettings", HTTP_POST, networkSettingsUpdate);

  server.on("/api/v1/intercomStatus", intercomStatus);
}