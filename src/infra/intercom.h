#include <Arduino.h>
#include <ArduinoJson.h>

#include "domain/stateMachineController.h"

void configureIntercom(String kmnModel, int firstApartment);
bool loadIntercomConfig();
void initIntercom();
IntercomConnectionStatus getIntercomStatus();
int getLastCalledNumber();
void getDefaultIntercomConf(JsonVariant& root);