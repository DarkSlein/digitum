#include "infra/relay.h"
#include "utils/utils.h"

void relayTurnOn() {
  digitalWrite(DRY_CONT_PIN, LOW);
}

void relayTurnOff() {
  digitalWrite(DRY_CONT_PIN, HIGH);
}

void switchRelay(SwitchDoorType type, DoorStatus status, int time) {
  switch (type) {
    case SwitchDoorType::ON_OFF:
      if (status == DoorStatus::OPENED) {
        relayTurnOn();
      } else {
        relayTurnOff();
      }
      break;

    case SwitchDoorType::JOGGING:
      if (status == DoorStatus::OPENED) {
        relayTurnOn();
        delay(secondsToMilliseconds(time));
        relayTurnOff();
      } else {
        relayTurnOff();
        delay(secondsToMilliseconds(time));
        relayTurnOn();
      }
      break;

    case SwitchDoorType::DELAY:
      delay(secondsToMilliseconds(time));
      relayTurnOn();
      break;
  }
}