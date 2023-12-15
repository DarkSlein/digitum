#pragma once

class StateMachineStrategy {
public:
  virtual void setup() = 0;
  virtual void updateStateMachine(int data) = 0;
  virtual ~StateMachineStrategy() {}
};