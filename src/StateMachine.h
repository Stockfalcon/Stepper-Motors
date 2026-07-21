#pragma once
#include "include/Communication Structures/EventGroups.h"
#include "include/MotorManager/MotorManager.h"

class StateMachine : public Task
{
public:
  StateMachine(MotorManager &controller, EventManager& events) : 
  motorController(controller),
  eventManager(events) {}

  void main() override;
  void onStateEnter(systemStates state);
  void onStateExit(systemStates state);
  void init();

private:
  systemStates currentState = MANUAL_MODE; // always start in manual mode
  systemStates lastState;
  MotorManager &motorController;
  TaskHandle_t stateMachineTask;
  EventManager &eventManager;
};