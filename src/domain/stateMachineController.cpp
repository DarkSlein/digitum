#include "domain/stateMachineController.h"

StateMachineController StateMachineController::instance;
StateMachineController& stateMachineController = StateMachineController::getInstance();

StateMachineController::StateMachineController() : _strategy(nullptr) {}

void StateMachineController::setStrategy(StateMachineStrategy* newStrategy) {
  if (_strategy) {
    delete _strategy;
  }

  _strategy = newStrategy;
  if (_strategy) {
    _strategy->setup();
  }
}

void StateMachineController::updateStateMachine(int data) {
  if (_strategy) {
    _strategy->updateStateMachine(data);
  }
}

StateMachineController::~StateMachineController() {
  if (_strategy) {
    delete _strategy;
  }
}

StateMachineController& StateMachineController::getInstance() {
  return instance;
}

IntercomConnectionStatus StateMachineController::getStatus() {
  return _strategy->getStatus();
}

int StateMachineController::getLastCalledNumber() {
  return _strategy->getFlat();
}

DoorStatus getDoorStatus() {
  return DoorStatus::CLOSED;
}