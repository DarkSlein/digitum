#pragma once

#include <Arduino.h>

String requestDataToStr(uint8_t *data, size_t len);
unsigned long secondsToMilliseconds(int seconds);