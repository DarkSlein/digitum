#include "domain/strategies/vizitStrategy.h"
#include "domain/services/intercomJournal.h"

VizitStrategy::VizitStrategy() {}

void VizitStrategy::setup() {
  _initStateMachine();
}

void VizitStrategy::updateStateMachine(int data) {
  switch (_currentState) {
    case NOT_CONNECTED:
      if (data == LOW) {} 
      else if (data == HIGH) {
        _changeState(CONNECTED);
      }
      break;

    case CONNECTED:
      if (data == LOW) {
        _countZeros++;
        if (_countZeros >= NOT_CONNECTED_THRESHOLD) {
          _changeState(NOT_CONNECTED);
        }
      } else if (data == HIGH) {
        if (_countZeros >= INITIALIZING_CALL_THRESHOLD) {
          _changeState(RECEIVING_FIRST_DIGIT);
        }
      }
      break;

    case RECEIVING_FIRST_DIGIT:
      _receiveDigit(data);

      if (data == LOW) {
        _countOnes = 0;
        _countZeros++;
      } else if (data == HIGH) {
        _countZeros = 0;
        _countOnes++;
        if (_countOnes >= DATA_RECEIVED_THESHOLD) {
          _firstDigitReceived();
          _changeState(RECEIVING_SECOND_DIGIT);
        }
        if (_countOnes >= CONNECTED_THRESHOLD) {
          _changeState(CONNECTED);
        }
      }
      break;

    case RECEIVING_SECOND_DIGIT:
      _receiveDigit(data);

      if (data == LOW) {
        _countOnes = 0;
        _countZeros++;
      } else if (data == HIGH) {
        _countZeros = 0;
        _countOnes++;
        if (_countOnes >= DATA_RECEIVED_THESHOLD) {
          _secondDigitReceived();
          _changeState(DATA_RECEIVED);
        }
        if (_countOnes >= CONNECTED_THRESHOLD) {
          _changeState(CONNECTED);
        }
      }
      break;

    case DATA_RECEIVED:
      if (data == LOW) {
        _countZeros++;
        if (_countZeros >= CALL_ENDED_THRESHOLD) {
          _changeState(CALL_ENDED);
        }
      } else if (data == HIGH) {
        _countOnes++;
        if (_countOnes >= CONNECTED_THRESHOLD) {
          _changeState(CONNECTED);
        }
      break;
      }

    case CALL_ENDED:
      if (data == LOW) {
        _countZeros++;
        if (_countZeros >= NOT_CONNECTED_THRESHOLD) {
          _changeState(NOT_CONNECTED);
        }
      } else if (data == HIGH) {
        _countOnes++;
        if (_countOnes >= CONNECTED_THRESHOLD) {
          _changeState(CONNECTED);
        }
      break;
      }
  }
}

void VizitStrategy::_resetCounters() {
  _countZeros = 0;
  _countOnes = 0;

  _previousData = 0;
  _dataLength = 1;
  _signalDuration = 0;
}

void VizitStrategy::_writeState(char* message) {
  println(message);
  publishToMQTT(STATE_MQTT_TOPIC, message);
}

void VizitStrategy::_receiveDigit(int data) {
  if (data != _previousData) {
    if (_previousData == HIGH) {
      //println("AAAA ", dataLength, " ", signalDuration);
      _dataLength++;
    }
    _signalDuration = 0;
  } else {
    _signalDuration++;
  }
  _previousData = data;
}

void VizitStrategy::_changeState(State state, bool resetCountersFlag) {
  _currentState = state;

  switch (state) {
    case NOT_CONNECTED:
      ledTurnOff();
      _writeState("not connected");
      _status = IntercomConnectionStatus::NOT_CONNECTED;
      break;
    case CONNECTED:
      ledTurnOn();
      _writeState("connected");
      _status = IntercomConnectionStatus::CONNECTED;
      break;
    case RECEIVING_FIRST_DIGIT:
      _writeState("receiving data");
      _status = IntercomConnectionStatus::RECEIVING_DATA;
      break;
  }

  if (resetCountersFlag)
    _resetCounters();
}

void VizitStrategy::_firstDigitReceived() {
  println("| 1 data length: ", _dataLength);
  _flat = _dataLength*10;
}

void VizitStrategy::_secondDigitReceived() {
  if (_dataLength == 11) {
    _dataLength = 1;
  }

  _flat += _dataLength;
  _flat -= 1;

  if (_flat > 100 && _flat < 110) {
    _flat -= 100;
  }

  println("| 2 data length: ", _dataLength);
  println("| flat: ", _flat);
  publishToMQTT(FLAT_NUMBER_MQTT_TOPIC, _flat);

  unsigned long createdAt = timeModule.getCurrentTime();
  addFlatToIntercomJournal(createdAt, _flat);
}

void VizitStrategy::_initStateMachine() {
  _changeState(CONNECTED);
}
