#include "domain/repos/userRepo.h"
#include "config/config.h"
#include "infra/fs.h"

#define MAX_USERS_SIZE 10
#define USER_SIZE 1024

User _jsonVariantToUser(const JsonVariant& json) {
  User user;

  user.username = json["username"].as<String>();
  user.admin = json["admin"].as<String>();

  return user;
}

StatusCode createUser(User user) {
  DynamicJsonDocument doc(USER_SIZE);

  readJsonVariantFromFile(USERS_PATH, doc);

  JsonArray root = doc.as<JsonArray>();

  for (const JsonVariant& userObj: root)
    if (userObj["username"] == user.username) 
      return StatusCode::CONFLICT;

  DynamicJsonDocument userDoc(USER_SIZE);
  JsonVariant userVariant = userDoc.as<JsonVariant>();

  userVariant["username"] = user.username;
  userVariant["admin"] = user.admin;
  
  root.add(userVariant);

  bool fileLoaded = writeJsonToFile(USERS_PATH, doc.as<JsonVariant>());

  return fileLoaded ? StatusCode::OK : StatusCode::INTERNAL_SERVER_ERROR;
}

User getFactoryUser() {
  return User{.username = FACTORY_ADMIN_USERNAME, .admin = FACTORY_ADMIN_PASSWORD};
}

User getUser(String username) {
  DynamicJsonDocument doc(USER_SIZE);

  if (!readJsonVariantFromFile(USERS_PATH, doc)) {
    User factoryUser = getFactoryUser();
    if (factoryUser.username == username)
      return factoryUser;
    else
      return User();
  }

  JsonArray root = doc.as<JsonArray>();

  for (const JsonVariant& userObj: root)
    if (userObj["username"] == username) 
      return _jsonVariantToUser(userObj);

  return User();
}

String getUsersJson() {
  DynamicJsonDocument doc(USER_SIZE*MAX_USERS_SIZE);
  if (!readJsonVariantFromFile(USERS_PATH, doc))
    return "{}";

  JsonArray root = doc.as<JsonArray>();

  String jsonString;
  serializeJson(root, jsonString);

  return jsonString;
}

StatusCode deleteUser(String username) {
  DynamicJsonDocument doc(USER_SIZE);
  if (!readJsonVariantFromFile(USERS_PATH, doc))
    return StatusCode::INTERNAL_SERVER_ERROR;

  JsonArray root = doc.as<JsonArray>();

  for (JsonArray::iterator it = root.begin(); it != root.end(); ++it) {
    if ((*it)["username"] == username) {
      root.remove(it);

      bool fileLoaded = writeJsonToFile(USERS_PATH, doc.as<JsonVariant>());
      return fileLoaded ? StatusCode::OK : StatusCode::INTERNAL_SERVER_ERROR;
    }
  }

  return StatusCode::NOT_FOUND;
}

StatusCode updateUser(String username, User user) {
  DynamicJsonDocument doc(USER_SIZE);
  if (!readJsonVariantFromFile(USERS_PATH, doc))
    return StatusCode::NOT_FOUND;

  JsonArray root = doc.as<JsonArray>();

  for (const JsonVariant& userObj: root) {
    if (userObj["username"] == username) {
      userObj["admin"] = user.admin;

      bool fileLoaded = writeJsonToFile(USERS_PATH, doc.as<JsonVariant>());
      return fileLoaded ? StatusCode::OK : StatusCode::INTERNAL_SERVER_ERROR;
    }
  }

  return StatusCode::NOT_FOUND;
}