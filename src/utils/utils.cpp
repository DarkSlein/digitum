#include "utils/utils.h"

String requestDataToStr(uint8_t *data, size_t len) {
  String str = "";

  for (size_t i = 0; i < len; i++) {
      str += (char)data[i];
  }

  return str;
}