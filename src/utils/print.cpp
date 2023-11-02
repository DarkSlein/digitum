#include <Arduino.h>

void print() {
  // Empty function to terminate recursion
}

// Function to print a newline
void println() {
  Serial.println();
}

String fstring(const char* format) {
  String result = format;
  return result;
}

void printf(const char* format) {
  Serial.print(fstring(format));
}