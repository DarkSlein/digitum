#include "utils/print.h"
#include "config/config.h"
#include "infra/mqtt.h"
#include "domain/stateMachine.h"

State currentState = NOT_CONNECTED;
int countZeros = 0;
int countOnes = 0;

int previousData = 0;
int dataLength = 0;
int signalDuration = 0;

void resetCounters() {
  countZeros = 0;
  countOnes = 0;

  previousData = 0;
  dataLength = 0;
  signalDuration = 0;
}

void writeState(char* message) {
  println(message);
  publishToMQTT(STATE_MQTT_TOPIC, message);
}

void updateStateMachine(int data) {
  switch (currentState) {
    case NOT_CONNECTED:
      if (data == 0) {
        // Stay in the NOT_CONNECTED state
      } else if (data == 1) {
        currentState = CONNECTED;
        writeState("connected");
      }
      break;

    case CONNECTED:
      if (data == 0) {
        countZeros++;
        if (countZeros >= NOT_CONNECTED_THRESHOLD) {
          currentState = NOT_CONNECTED;
          writeState("not connected");
          resetCounters();
        }
      } else if (data == 1) {
        if (countZeros >= INITIALIZING_CALL_THRESHOLD) {
          currentState = RECEIVING_DATA;
          writeState("receiving data");
          resetCounters();
        }
      }
      break;

    case RECEIVING_DATA:
      if (data != previousData) {
        if (previousData == HIGH) {
          dataLength++;
        }
        signalDuration = 0;
      } else {
        signalDuration++;
      }
      previousData = data;
      if (data == 0) {
        countOnes = 0;
        countZeros++;
        if (countZeros >= DATA_RECEIVED_THESHOLD) {
          println("| data length: ", dataLength);
          println("| flat: ", dataLength/2);
          publishToMQTT(FLAT_NUMBER_MQTT_TOPIC, dataLength/2);
          
          currentState = DATA_RECEIVED;
          resetCounters();
        }
      } else if (data == 1) {
        countZeros = 0;
        countOnes++;
        if (countOnes >= CONNECTED_THRESHOLD) {
          currentState = CONNECTED;
          writeState("connected");
          resetCounters();
        }
      }
      break;

    case DATA_RECEIVED:
      if (data == 0) {
        countZeros++;
        if (countZeros >= CALL_ENDED_THRESHOLD) {
          currentState = CALL_ENDED;
          writeState("call ended");
          resetCounters();
        }
      } else if (data == 1) {
        countOnes++;
        if (countOnes >= CONNECTED_THRESHOLD) {
          currentState = CONNECTED;
          writeState("connected");
          resetCounters();
        }
      break;
      }

    case CALL_ENDED:
      if (data == 0) {
        countZeros++;
        if (countZeros >= NOT_CONNECTED_THRESHOLD) {
          currentState = NOT_CONNECTED;
          writeState("not connected");
          resetCounters();
        }
      } else if (data == 1) {
        countOnes++;
        if (countOnes >= CONNECTED_THRESHOLD) {
          currentState = CONNECTED;
          writeState("connected");
          resetCounters();
        }
      break;
      }
  }
}