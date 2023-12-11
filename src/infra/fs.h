#include <LittleFS.h>
#include <ArduinoJson.h>
#include "utils/print.h"

void initFileSystem();
bool readJsonVariantFromFile(const char* filename, DynamicJsonDocument& jsonDoc);
bool writeJsonVariantToFile(const char* filename, JsonVariant& jsonObj);
void deleteFilesInDir(const char* path);