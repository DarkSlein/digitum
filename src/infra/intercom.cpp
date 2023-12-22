#include "infra/intercom.h"

extern StateMachineController& stateMachineController;

void configureIntercom(String kmnModel, int firstAppartment) {

}

IntercomConnectionStatus getIntercomStatus() {
  return stateMachineController.getStatus();
}

int getLastCalledNumber() {
  return stateMachineController.getLastCalledNumber();
}
