#pragma once
#include "include/Communication Structures/EventGroups.h"

class StateMachine
{
public:
  static void systemStateSwitcherTask(void *pvParameters);
  void systemStateSwitcher();
  void onStateEnter(systemStates state);
  void onStateExit(systemStates state);

private:
  systemStates currentState = MANUAL_MODE; // always start in manual mode
  systemStates lastState;
};