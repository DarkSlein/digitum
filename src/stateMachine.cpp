#include "stateMachine.h"

void updateStateMachine(int data) {
  switch (currentState) {
    case NOT_CONNECTED:
      if (data == 0) {
        // Stay in the NOT_CONNECTED state
      } else if (data == 1) {
        currentState = CONNECTED;
        println("connected");
      }
      break;

    case CONNECTED:
      if (data == 0) {
        countZeros++;
        if (countZeros >= NOT_CONNECTED_THRESHOLD) {
          currentState = NOT_CONNECTED;
          println("not connected");
          resetCounters();
        }
      } else if (data == 1) {
        if (countZeros >= INITIALIZING_CALL_THRESHOLD) {
          currentState = RECEIVING_DATA;
          println("receiving data");
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
          //sendDataToMQTT(dataLength/2);

          currentState = DATA_RECEIVED;
          println("data received");
          resetCounters();
        }
      } else if (data == 1) {
        countZeros = 0;
        countOnes++;
        if (countOnes >= CONNECTED_THRESHOLD) {
          currentState = CONNECTED;
          println("connected");
          resetCounters();
        }
      }
      break;

    case DATA_RECEIVED:
      if (data == 0) {
        countZeros++;
        if (countZeros >= CALL_ENDED_THRESHOLD) {
          currentState = CALL_ENDED;
          println("call ended");
          resetCounters();
        }
      } else if (data == 1) {
        countOnes++;
        if (countOnes >= CONNECTED_THRESHOLD) {
          currentState = CONNECTED;
          println("connected");
          resetCounters();
        }
      break;
      }

    case CALL_ENDED:
      if (data == 0) {
        countZeros++;
        if (countZeros >= NOT_CONNECTED_THRESHOLD) {
          currentState = NOT_CONNECTED;
          println("not connected");
          resetCounters();
        }
      } else if (data == 1) {
        countOnes++;
        if (countOnes >= CONNECTED_THRESHOLD) {
          currentState = CONNECTED;
          println("connected");
          resetCounters();
        }
      break;
      }
  }
}