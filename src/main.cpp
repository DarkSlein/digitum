#include <Arduino.h>
#include <ETH.h>
#include <SPI.h>
#include <SD.h>
#include <HTTPClient.h>
#include <IPAddress.h>
#include <AsyncMqttClient.h>

#include "utils/print.h"
#include "config/config.h"

#include "infra/eth.h"
#include "infra/mqtt.h"
#include "infra/led.h"
#include "infra/relay.h"
#include "infra/fs.h"
#include "infra/httpServer.h"

#include "domain/stateMachine.h"
#include "app/routes.h"

uint32_t lastMillis;
uint64_t lastMicros;

bool eth_connected = false;

int data = 0;

void setup() {
  Serial.begin(115200);

  WiFi.onEvent(WiFiEvent);  

  pinMode(DATA_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(DRY_CONT_PIN, OUTPUT);

  digitalWrite(LED_PIN, HIGH);

  relayTurnOn();
  delay(2000);
  relayTurnOff();
  delay(2000);
  relayTurnOn();
  delay(2000);

  initFileSystem();
  initEth();
  initMQTT();
  initRoutes();
  initHttpServer();
}

void loop() {
  if (lastMicros < micros()) {
    data = digitalRead(DATA_PIN);
    updateStateMachine(data);

    if (PRINT_RAW_SIGNAL_FLAG)
      printf("{}", data);

    lastMillis = micros() + DATA_PERIOD;
  }

  if (eth_connected) {
    if (lastMillis < millis()) {;
      lastMillis = millis() + 3000;
    }
  }
}