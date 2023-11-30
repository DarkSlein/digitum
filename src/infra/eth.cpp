#include <ArduinoJson.h>
#include "infra/eth.h"
#include "infra/fs.h"
#include "infra/mqtt.h"
#include "config/config.h"
#include "utils/print.h"

extern bool eth_connected;
TimerHandle_t ethReconnectTimer;

void WiFiEvent(WiFiEvent_t event)
{
#if ESP_IDF_VERSION_MAJOR > 3
    switch (event) {
    case ARDUINO_EVENT_ETH_START:
        Serial.println("ETH Started");
        // set eth hostname here
        ETH.setHostname("esp32-ethernet");
        break;
    case ARDUINO_EVENT_ETH_CONNECTED:
        Serial.println("ETH Connected");
        break;
    case ARDUINO_EVENT_ETH_GOT_IP:
        Serial.print("ETH MAC: ");
        Serial.print(ETH.macAddress());
        Serial.print(", IPv4: ");
        Serial.print(ETH.localIP());
        if (ETH.fullDuplex()) {
            Serial.print(", FULL_DUPLEX");
        }
        Serial.print(", ");
        Serial.print(ETH.linkSpeed());
        Serial.println("Mbps");
        eth_connected = true;
        delay(2000);
        connectToMqtt();
        break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
        Serial.println("ETH Disconnected");
        eth_connected = false;
        break;
    case ARDUINO_EVENT_ETH_STOP:
        Serial.println("ETH Stopped");
        eth_connected = false;
        break;
    default:
        break;
    }
#elif
    switch (event) {
    case SYSTEM_EVENT_ETH_START:
        Serial.println("ETH Started");
        // set eth hostname here
        ETH.setHostname("esp32-ethernet");
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        Serial.println("ETH Connected");
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:
        Serial.print("ETH MAC: ");
        Serial.print(ETH.macAddress());
        Serial.print(", IPv4: ");
        Serial.print(ETH.localIP());
        if (ETH.fullDuplex()) {
            Serial.print(", FULL_DUPLEX");
        }
        Serial.print(", ");
        Serial.print(ETH.linkSpeed());
        Serial.println("Mbps");
        eth_connected = true;
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        Serial.println("ETH Disconnected");
        eth_connected = false;
        break;
    case SYSTEM_EVENT_ETH_STOP:
        Serial.println("ETH Stopped");
        eth_connected = false;
        break;
    default:
        break;
    }
#endif
}

bool loadEthConfig() {
  DynamicJsonDocument doc(1024);

  bool success = readJsonVariantFromFile(NETWORK_SETTINGS_PATH, doc);

  if (!success)
    return false;

  JsonVariant root = doc.as<JsonVariant>();

  setEthConfig(
    root["static_ip_config"].as<bool>(),
    root["local_ip"].as<String>(),
    root["gateway_ip"].as<String>(),
    root["subnet_mask"].as<String>(),
    root["dns_ip"].as<String>()
  );

  return true;
}

void initEth() {
    pinMode(NRST, OUTPUT);
    digitalWrite(NRST, 0);
    delay(200);
    digitalWrite(NRST, 1);
    delay(200);
    digitalWrite(NRST, 0);
    delay(200);
    digitalWrite(NRST, 1);
    delay(200);

    ETH.begin(ETH_ADDR,
              ETH_POWER_PIN,
              ETH_MDC_PIN,
              ETH_MDIO_PIN,
              ETH_TYPE,
              ETH_CLK_MODE);

    if (!loadEthConfig() && DEFAULT_STATIC_LOCAL_IP) {
        ETH.config(
            DEFAULT_STATIC_LOCAL_IP,
            DEFAULT_STATIC_GATEWAY,
            DEFAULT_STATIC_SUBNET
        );
    }
}

void testClient(const char * host, uint16_t port)
{
  Serial.print("\nconnecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  client.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (client.connected() && !client.available());
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("closing connection\n");
  client.stop();
}

void setEthConfig(bool isStatic, String localIp, String gateway,
String subnet, String dns1, String dns2) {
    IPAddress newLocalIp = IPAddress();
    IPAddress newGateway = IPAddress();
    IPAddress newSubnet = IPAddress();
    IPAddress newDns1 = IPAddress();
    IPAddress newDns2 = IPAddress();

    newLocalIp.fromString(localIp);
    newGateway.fromString(gateway);
    newSubnet.fromString(subnet);
    newDns1.fromString(dns1);
    newDns2.fromString(dns2);

    if (isStatic)
        ETH.config(
            newLocalIp,
            newGateway,
            newSubnet,
            newDns1,
            newDns2
        );
    else {
        ETH.config((uint32_t)0, (uint32_t)0, (uint32_t)0);
    }
}