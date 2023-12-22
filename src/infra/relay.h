#pragma once
#include <Arduino.h>
#include "config/config.h"

enum class SwitchDoorType { ON_OFF, JOGGING, DELAY };
enum class DoorStatus { OPENED, CLOSED };

void relayTurnOn();
void relayTurnOff();
void switchRelay(SwitchDoorType type, DoorStatus status, int time);