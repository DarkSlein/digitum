#include <LittleFS.h>
#include <ArduinoJson.h>
#include "utils/print.h"

void initFileSystem();
bool readJsonVariantFromFile(const char* filename, DynamicJsonDocument& jsonDoc);
bool writeJsonToFile(const char* file_path, const JsonVariant& jsonValue);
void deleteFilesInDir(const char* path);