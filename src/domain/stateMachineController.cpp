#include "domain/stateMachineController.h"

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
  // Cleanup the strategy in the destructor
  if (_strategy) {
    delete _strategy;
  }
}