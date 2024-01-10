#include "domain/services/intercomJournal.h"
#include "utils/print.h"

IntercomJournal _intercomJournal;

void addFlatToIntercomJournal(unsigned long createdAt, int appartment) {
  for (int i = INTERCOM_JOURNAL_FLATS_NUMBER - 1; i > 0; --i) {
    _intercomJournal.notes[i] = _intercomJournal.notes[i - 1];
  }

  _intercomJournal.notes[0].createdAt = createdAt;
  _intercomJournal.notes[0].appartment = appartment;
}

JsonArray getIntercomJournalAsJson(JsonArray& flats) {
  for (int i = 0; i < INTERCOM_JOURNAL_FLATS_NUMBER; ++i) {
    unsigned long createdAt = _intercomJournal.notes[i].createdAt;
    int& appartment = _intercomJournal.notes[i].appartment;

    if (createdAt && appartment) {
      unsigned long timeDifference = timeModule.getTimeDifference(createdAt);
      String formatedTimeDifference = "\n(" + timeModule.getFormattedTimeAgo(timeDifference) + ")";

      JsonObject flat = flats.createNestedObject();
      flat["createdAt"] = timeModule.getFormattedTime(createdAt) + formatedTimeDifference;
      flat["appartment"] = appartment;
    }
  }

  return flats;
}