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
#include "domain/stateMachine.h"

uint32_t lastMillis;
uint64_t lastMicros;

bool eth_connected = false;

int data = 0;

void setup() {
    Serial.begin(115200);

    WiFi.onEvent(WiFiEvent);

    connectEth();
    initMQTT();

    pinMode(DATA_PIN, INPUT);

    pinMode(DRY_CONT_PIN, OUTPUT);
    digitalWrite(DRY_CONT_PIN, 0);
    delay(2000);
    digitalWrite(DRY_CONT_PIN, 1);
    delay(2000);
    digitalWrite(DRY_CONT_PIN, 0);
    delay(2000);
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