#include "infra/relay.h"

void relayTurnOn() {
  digitalWrite(DRY_CONT_PIN, LOW);
}

void relayTurnOff() {
  digitalWrite(DRY_CONT_PIN, HIGH);
}