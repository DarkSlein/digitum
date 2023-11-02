#ifndef UTILS_PRINT_H
#define UTILS_PRINT_H

#include <Arduino.h>

void print();

// Overloaded function to print a single value
template <typename T>
void print(T value);

// Recursive function to print multiple values
template <typename T, typename... Args>
void print(T value, Args... args);

// Function to print a newline
void println();

// Overloaded function to print a single value and a newline
template <typename T>
void println(T value);

// Recursive function to print multiple values and a newline
template <typename T, typename... Args>
void println(T value, Args... args);

// Convenience functions for easy usage
template <typename... Args>
void print(Args... args);

template <typename... Args>
void println(Args... args);

String fstring(const char* format);

template <typename... Args>
String fstring(const char* format, Args... args);

void printf(const char* format);

template <typename... Args>
void printf(const char* format, Args... args);

#endif // UTILS_PRINT_H