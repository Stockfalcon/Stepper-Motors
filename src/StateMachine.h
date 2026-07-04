#pragma once
#include "include/Communication Structures/EventGroups.h"
#include "include/MotorManager/MotorManager.h"

class StateMachine
{
public:
StateMachine(MotorController  &controller) : motorController(controller){} //?comma separated for multiple
  static void systemStateSwitcherTask(void *pvParameters);
  void systemStateSwitcher();
  void onStateEnter(systemStates state);
  void onStateExit(systemStates state);

private:
  systemStates currentState = MANUAL_MODE; // always start in manual mode
  systemStates lastState;
  MotorController motorController;
};