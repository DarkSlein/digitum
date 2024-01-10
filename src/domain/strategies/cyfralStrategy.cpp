#include "domain/strategies/cyfralStrategy.h"
#include "domain/services/intercomJournal.h"

CyfralStrategy::CyfralStrategy() {}

void CyfralStrategy::setup() {
  _initStateMachine();
}

void CyfralStrategy::updateStateMachine(int data) {
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
          _changeState(RECEIVING_DATA);
        }
      }
      break;

    case RECEIVING_DATA:
      _receiveData(data);

      if (data == LOW) {
        _countOnes = 0;
        _countZeros++;
        if (_countZeros >= DATA_RECEIVED_THESHOLD) {
          _flatReceived();
          _changeState(DATA_RECEIVED);
        }
      } else if (data == HIGH) {
        _countZeros = 0;
        _countOnes++;
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

void CyfralStrategy::_resetCounters() {
  _countZeros = 0;
  _countOnes = 0;

  _previousData = 0;
  _dataLength = 0;
  _signalDuration = 0;
}

void CyfralStrategy::_writeState(char* message) {
  println(message);
  publishToMQTT(STATE_MQTT_TOPIC, message);
}

void CyfralStrategy::_receiveData(int data) {
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

void CyfralStrategy::_changeState(State state, bool resetCountersFlag) {
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
    case RECEIVING_DATA:
      _writeState("receiving data");
      _status = IntercomConnectionStatus::RECEIVING_DATA;
      break;
  }

  if (resetCountersFlag)
    _resetCounters();
}

void CyfralStrategy::_flatReceived() {
  _flat = _dataLength/2;

  if (_flat < 1)
    return;

  println("| data length: ", _dataLength);
  println("| flat: ", _flat);
  publishToMQTT(FLAT_NUMBER_MQTT_TOPIC, _flat);

  unsigned long createdAt = timeModule.getCurrentTime();
  addFlatToIntercomJournal(createdAt, _flat);
}

void CyfralStrategy::_initStateMachine() {
  _changeState(CONNECTED);
}
