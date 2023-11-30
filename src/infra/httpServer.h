#include <Arduino.h>
#include <ETH.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>

void redirectToIndex(AsyncWebServerRequest *request);
AsyncWebServer& getServer();
void initHttpServer();