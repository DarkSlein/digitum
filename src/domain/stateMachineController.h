#pragma once

#include "infra/relay.h"
#include "domain/stateMachineStrategy.h"

class StateMachineController {
private:
  static StateMachineController instance;
  StateMachineStrategy* _strategy;

  StateMachineController();
  StateMachineController(const StateMachineController&) = delete;
  StateMachineController& operator=(const StateMachineController&) = delete;
  ~StateMachineController();

public:
  void setStrategy(StateMachineStrategy* newStrategy);
  void updateStateMachine(int data);
  static StateMachineController& getInstance();

  IntercomConnectionStatus getStatus();
  int getLastCalledNumber();
  DoorStatus getDoorStatus();
};