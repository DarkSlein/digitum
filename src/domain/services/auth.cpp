#include "domain/services/auth.h"
#include "domain/entities/user.h"
#include "domain/repos/userRepo.h"
#include "config/config.h"
#include "utils/print.h"

ArduinoJWT jwtService(JWT_SECRET_KEY);

StatusCode getAccessToken(String username, String password, String& accessToken) {
  DynamicJsonDocument jsonDoc(MAX_LOG_IN_SIZE);
  JsonVariant root = jsonDoc.as<JsonVariant>();

  User user = getUser(username);

  if (user.username == "")
    return StatusCode::NOT_FOUND;

  root["username"] = username;
  root["admin"] = user.admin;

  String jsonString;
  serializeJson(root, jsonString);

  accessToken = jwtService.encodeJWT(jsonString);
  return StatusCode::OK;
}