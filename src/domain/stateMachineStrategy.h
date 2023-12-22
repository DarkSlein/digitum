#pragma once

enum class IntercomConnectionStatus { CONNECTED, NOT_CONNECTED, RECEIVING_DATA };

class StateMachineStrategy {
protected:
  int _flat = 0;

public:
  IntercomConnectionStatus _status = IntercomConnectionStatus::NOT_CONNECTED;

  virtual void setup() = 0;
  virtual void updateStateMachine(int data) = 0;
  virtual ~StateMachineStrategy() {}

  int getFlat() {
    return _flat;
  }

  IntercomConnectionStatus getStatus() {
    return _status;
  }
};