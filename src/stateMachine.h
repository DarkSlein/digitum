#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <Arduino.h>
#include "utils/print.cpp"

#define CONNECTED_THRESHOLD 50000
#define NOT_CONNECTED_THRESHOLD 50000
#define INITIALIZING_CALL_THRESHOLD 15000
#define DATA_RECEIVED_THESHOLD 30000
#define CALL_ENDED_THRESHOLD 10000

enum State {
  NOT_CONNECTED,
  CONNECTED,
  RECEIVING_DATA,
  DATA_RECEIVED,
  CALL_ENDED
};

int data = 0;
State currentState = NOT_CONNECTED;
int countZeros = 0;
int countOnes = 0;

int previousData = 0;
int dataLength = 0;
int signalDuration = 0;

void resetCounters();
void updateStateMachine(int data);

#endif // STATE_MACHINE_H