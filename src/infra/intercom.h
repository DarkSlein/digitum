#include <Arduino.h>
#include "domain/stateMachineController.h"

void configureIntercom(String kmnModel, int firstAppartment);
IntercomConnectionStatus getIntercomStatus();
int getLastCalledNumber();