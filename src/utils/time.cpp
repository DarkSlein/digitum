#include <TimeLib.h>
#include "utils/time.h"

Time& timeModule = Time::getInstance();

Time::Time() {
  initialTime = 0;
}

Time& Time::getInstance() {
  static Time instance;
  return instance;
}

void Time::setCurrentTime(unsigned long currentTimeMillis) {
  initialTime = currentTimeMillis;
}

unsigned long Time::getCurrentTime() const {
  return millis() + initialTime;
}

unsigned long Time::getTimeDifference(unsigned long otherTimeMillis) const {
  return millis() - otherTimeMillis;
}

String Time::getFormattedTime(unsigned long timeMillis) const {
  time_t currentTime = timeMillis / 1000;

  struct tm *timeStruct = gmtime(&currentTime);

  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
           timeStruct->tm_year + 1900, timeStruct->tm_mon + 1, timeStruct->tm_mday,
           timeStruct->tm_hour, timeStruct->tm_min, timeStruct->tm_sec);

  return String(buffer);
}

String Time::getFormattedTimeAgo(unsigned long timeMillis) const {
  time_t currentTime = timeMillis / 1000;

  struct tm *timeStruct = gmtime(&currentTime);

  if (timeStruct->tm_yday >= 0) {
    return String(timeStruct->tm_yday) + " days ago";
  } else if (timeStruct->tm_hour > 0) {
    return String(timeStruct->tm_hour) + " hours ago";
  } else if (timeStruct->tm_min > 0) {
    return String(timeStruct->tm_min) + " minutes ago";
  } else {
    return String(timeStruct->tm_sec) + " seconds ago";
  }
}