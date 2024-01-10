#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "config/config.h"
#include "utils/time.h"

extern Time& timeModule;

struct Flat {
  unsigned long createdAt;
  int appartment;
};

struct IntercomJournal {
  Flat notes[INTERCOM_JOURNAL_FLATS_NUMBER];
};

void addFlatToIntercomJournal(unsigned long createdAt, int appartment);
JsonArray getIntercomJournalAsJson(JsonArray& flats);