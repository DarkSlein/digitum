#pragma once

#include "domain/stateMachineStrategy.h"

class StateMachineController {
private:
  StateMachineStrategy* _strategy;

public:
  StateMachineController();
  void setStrategy(StateMachineStrategy* newStrategy);
  void updateStateMachine(int data);
  ~StateMachineController();
};