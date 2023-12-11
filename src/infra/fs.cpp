#include "infra/fs.h"

void initFileSystem() {
  if (!LittleFS.begin(true))
  {
#ifdef VERBOSE
    println("An Error has occurred while mounting LittleFS");
#endif
    return;
  }
}

bool readJsonVariantFromFile(const char* file_path, DynamicJsonDocument& jsonDoc) {
  if (!LittleFS.exists(file_path)) {
    println("Config file does not exist: ", file_path);
    return false;
  }

  File file = LittleFS.open(file_path, "r");

  if (!file) {
    println("Failed to open file for reading: ", file_path);
    return false;
  }

  String fileContent = file.readString(); 
  println("Reading config: ", file_path, ", content:", fileContent);

  DeserializationError error = deserializeJson(jsonDoc, fileContent);

  file.close();

  if (error) {
    println("Error parsing JSON: ", String(error.c_str()));
    return false;
  }

  return true;
}

bool createFolderIfNotExists(const char* file_path) {
  char folder[64];
  strcpy(folder, file_path);

  char* lastSlash = strrchr(folder, '/');
  if (lastSlash != nullptr) {
    *lastSlash = '\0';
  }

  return LittleFS.mkdir(folder);
}

bool writeJsonVariantToFile(const char* file_path, JsonVariant& jsonVariant) {
  createFolderIfNotExists(file_path);

  File file = LittleFS.open(file_path, "w");

  if (!file) {
    println("Failed to open file for writing: ", file_path);
    return false;
  }

  String jsonString;
  serializeJson(jsonVariant, jsonString);
  println("Writing config: ", file_path, ", content:", jsonString);
  file.print(jsonString);

  file.close();

  return true;
}

void deleteFilesInDir(const char* path) {
  File dir = LittleFS.open(path);
  if (!dir.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = dir.openNextFile();
  while (file) {
    String fullPath = String(path) + '/' +  String(file.name());
    if (file.isDirectory()) {
      deleteFilesInDir(fullPath.c_str());
    } else {
      file.close();
      LittleFS.remove(fullPath);
    }

    file = dir.openNextFile();
  }
  dir.close();
}
