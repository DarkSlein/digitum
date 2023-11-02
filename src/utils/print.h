#pragma once

#include <Arduino.h>

void print();

// Overloaded function to print a single value
template <typename T>
void print(T value) {
  Serial.print(value);
}

// Recursive function to print multiple values
template <typename T, typename... Args>
void print(T value, Args... args) {
  Serial.print(value);
  print(args...); // Recursively call print for the remaining arguments
}

// Function to print a newline
void println();

// Overloaded function to print a single value and a newline
template <typename T>
void println(T value) {
  Serial.println(value);
}

// Recursive function to print multiple values and a newline
template <typename T, typename... Args>
void println(T value, Args... args) {
  Serial.print(value);
  println(args...); // Recursively call println for the remaining arguments
}

// Convenience functions for easy usage
template <typename... Args>
void print(Args... args) {
  print(args...);
}

template <typename... Args>
void println(Args... args) {
  println(args...);
}

String fstring(const char* format);

template <typename... Args>
String fstring(const char* format, Args... args) {
  String result = "";
  while (*format) {
    if (*format == '{' && *(format + 1) == '}' && sizeof...(args) > 0) {
      result += String(args...);
      format += 2; // Skip the "{}" in the format string
    } else {
      result += *format;
      format++;
    }
  }
  return result;
}

void printf(const char* format);

template <typename... Args>
void printf(const char* format, Args... args) {
  Serial.print(fstring(format, args...));
}