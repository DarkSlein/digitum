#include "utils/print.h"
#include "utils/time.h"
#include "config/config.h"
#include "infra/mqtt.h"
#include "infra/led.h"
#include "domain/stateMachineStrategy.h"

extern Time& timeModule;

class CyfralStrategy : public StateMachineStrategy {
private:
  enum State {
    NOT_CONNECTED,
    CONNECTED,
    RECEIVING_DATA,
    DATA_RECEIVED,
    CALL_ENDED
  };

  const int CONNECTED_THRESHOLD = 50000;
  const int NOT_CONNECTED_THRESHOLD = 50000;
  const int INITIALIZING_CALL_THRESHOLD = 15000;
  const int DATA_RECEIVED_THESHOLD = 30000;
  const int CALL_ENDED_THRESHOLD = 10000;

  State _currentState = NOT_CONNECTED;
  int _countZeros = 0;
  int _countOnes = 0;

  int _previousData = 0;
  int _dataLength = 0;
  int _signalDuration = 0;

  void _resetCounters();
  void _writeState(char* message);
  void _receiveData(int data);
  void _changeState(State state, bool resetCountersFlag = true);
  void _flatReceived();
  void _initStateMachine();

public:
  CyfralStrategy();
  void setup() override;
  void updateStateMachine(int data) override;
};
