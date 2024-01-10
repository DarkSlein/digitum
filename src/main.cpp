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
#include "infra/intercom.h"

#include "app/routes.h"

#include "domain/stateMachineController.h"
#include "domain/strategies/cyfralStrategy.h"

uint32_t lastMillis;
uint64_t lastMicros;

int data = 0;

bool flag = false;
int zeros = 0;
int ones = 0;

extern StateMachineController& stateMachineController;

void IRAM_ATTR one() {
  flag = true;
  //zeros = 0;
  ones += 1;
}

void IRAM_ATTR zero() {
  flag = false;
  //ones = 0;
  zeros += 1;
}

void setup() {
  Serial.begin(115200);

  WiFi.onEvent(WiFiEvent);  

  pinMode(DATA_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(DRY_CONT_PIN, OUTPUT);
  attachInterrupt(DATA_PIN, one, RISING);
  attachInterrupt(DATA_PIN, zero, FALLING);

  digitalWrite(LED_PIN, HIGH);

  relayTurnOn();
  delay(2000);
  relayTurnOff();
  delay(2000);
  relayTurnOn();
  delay(2000);

  //initStateMachine();
  initFileSystem();
  initEth();
  initMQTT();
  initRoutes();
  initHttpServer();
  initIntercom();
}

void loop() {
  if (lastMicros < micros()) {
    data = digitalRead(DATA_PIN);
    stateMachineController.updateStateMachine(data);

    if (PRINT_RAW_SIGNAL_FLAG)
      printf("{}", data);

    lastMicros = micros() + DATA_PERIOD;
  }

  if (true) {
    if (lastMillis < millis()) {
      if (getMqttEnabled() && !getMqttConnected()) {
        reconnectMQTTIfNeeded();
      }
      /*if (flag) {
        changeState(RECEIVING_DATA);
        flag = false;
      } else {
        changeState(CONNECTED);
        flag = true;
      }*/
      lastMillis = millis() + 3000;
    }
  }
}