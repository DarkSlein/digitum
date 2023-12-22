#pragma once

#include <Arduino.h>

class Time {
public:
  static Time& getInstance();
  void setCurrentTime(unsigned long currentTimeMillis);
  unsigned long getCurrentTime() const;
  unsigned long getTimeDifference(unsigned long otherTimeMillis) const;
  String getFormattedTime(unsigned long timeMillis) const;
  String getFormattedTimeAgo(unsigned long timeMillis) const;

private:
  Time();
  unsigned long initialTime;
};