#pragma once
#include "include/ButtonManager/ButtonManager.h"
#include "include/MotorManager/MotorManager.h"
#include "include/Communication Structures/EventGroups.h"
#include "StateMachine.h"



class Application{
  public:
  Application() : stateManager(), buttonManager(), motorManager(), stateMachine(motorManager){}
  void init();
  private:
  StateManager stateManager;
  ButtonManager buttonManager;
  MotorManager motorManager;
  StateMachine stateMachine;
};
  