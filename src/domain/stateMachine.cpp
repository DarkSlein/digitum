#include "utils/print.h"
#include "config/config.h"
#include "infra/mqtt.h"
#include "infra/led.h"
#include "domain/stateMachine.h"

State currentState = NOT_CONNECTED;
int countZeros = 0;
int countOnes = 0;

int previousData = 0;
int dataLength = 0;
int signalDuration = 0;

int flat = 0;

void resetCounters() {
  countZeros = 0;
  countOnes = 0;

  previousData = 0;
  dataLength = 1;
  signalDuration = 0;
}

void writeState(char* message) {
  //println(message);
  publishToMQTT(STATE_MQTT_TOPIC, message);
}

void receiveDigit(int data) {
  if (data != previousData) {
    if (previousData == HIGH) {
      //println("AAAA ", dataLength, " ", signalDuration);
      dataLength++;
    }
    signalDuration = 0;
  } else {
    signalDuration++;
  }
  previousData = data;
}

void changeState(State state, bool resetCountersFlag=true) {
  currentState = state;

  switch (state) {
    case NOT_CONNECTED:
      ledTurnOff();
      writeState("not connected");
      break;
    case CONNECTED:
      ledTurnOn();
      writeState("connected");
      break;
    case RECEIVING_FIRST_DIGIT:
      writeState("receiving data");
      break;
  }

  if (resetCountersFlag)
    resetCounters();
}

void firstDigitReceived() {
  println("| 1 data length: ", dataLength);
  flat = dataLength*10;
}

void secondDigitReceived() {
  if (dataLength == 11) {
    dataLength = 1;
  }

  flat += dataLength;
  flat -= 1;

  if (flat > 100 && flat < 110) {
    flat -= 100;
  }

  println("| 2 data length: ", dataLength);
  println("| flat: ", flat);
  publishToMQTT(FLAT_NUMBER_MQTT_TOPIC, flat);
}

void updateStateMachine(int data) {
  switch (currentState) {
    case NOT_CONNECTED:
      if (data == LOW) {} 
      else if (data == HIGH) {
        changeState(CONNECTED);
      }
      break;

    case CONNECTED:
      if (data == LOW) {
        countZeros++;
        if (countZeros >= NOT_CONNECTED_THRESHOLD) {
          changeState(NOT_CONNECTED);
        }
      } else if (data == HIGH) {
        if (countZeros >= INITIALIZING_CALL_THRESHOLD) {
          changeState(RECEIVING_FIRST_DIGIT);
        }
      }
      break;

    case RECEIVING_FIRST_DIGIT:
      receiveDigit(data);

      if (data == LOW) {
        countOnes = 0;
        countZeros++;
      } else if (data == HIGH) {
        countZeros = 0;
        countOnes++;
        if (countOnes >= DATA_RECEIVED_THESHOLD) {
          firstDigitReceived();
          changeState(RECEIVING_SECOND_DIGIT);
        }
        if (countOnes >= CONNECTED_THRESHOLD) {
          changeState(CONNECTED);
        }
      }
      break;

    case RECEIVING_SECOND_DIGIT:
      receiveDigit(data);

      if (data == LOW) {
        countOnes = 0;
        countZeros++;
      } else if (data == HIGH) {
        countZeros = 0;
        countOnes++;
        if (countOnes >= DATA_RECEIVED_THESHOLD) {
          secondDigitReceived();
          changeState(DATA_RECEIVED);
        }
        if (countOnes >= CONNECTED_THRESHOLD) {
          changeState(CONNECTED);
        }
      }
      break;

    case DATA_RECEIVED:
      if (data == LOW) {
        countZeros++;
        if (countZeros >= CALL_ENDED_THRESHOLD) {
          changeState(CALL_ENDED);
        }
      } else if (data == HIGH) {
        countOnes++;
        if (countOnes >= CONNECTED_THRESHOLD) {
          changeState(CONNECTED);
        }
      break;
      }

    case CALL_ENDED:
      if (data == LOW) {
        countZeros++;
        if (countZeros >= NOT_CONNECTED_THRESHOLD) {
          changeState(NOT_CONNECTED);
        }
      } else if (data == HIGH) {
        countOnes++;
        if (countOnes >= CONNECTED_THRESHOLD) {
          changeState(CONNECTED);
        }
      break;
      }
  }
}

void initStateMachine() {
  changeState(CONNECTED);
}