#include <Arduino.h>

#include "config/config.h"
#include "infra/led.h"

void ledTurnOn() {
  digitalWrite(LED_PIN, LOW);
}

void ledTurnOff() {
  digitalWrite(LED_PIN, HIGH);
}