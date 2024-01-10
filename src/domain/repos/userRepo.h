#pragma once

#include <Arduino.h>

#include "utils/errorCodes.h"
#include "domain/services/auth.h"
#include "domain/entities/user.h"

StatusCode createUser(User user);
User getUser(String username);
String getUsersJson();
StatusCode deleteUser(String username);
StatusCode updateUser(String username, User user);