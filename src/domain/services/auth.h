#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoJWT.h>

#include "utils/errorCodes.h"

StatusCode getAccessToken(String username, String password, String& accessToken);