#pragma once
#include "include/ButtonManager/ButtonManager.h"
#include "include/MotorManager/MotorManager.h"
#include "include/Communication Structures/EventGroups.h"
#include "StateMachine.h"



class Application{
  public:
  Application() : eventManager(), buttonManager(), motorManager(), stateMachine(motorManager, eventManager){}
  void init();
  private:
  EventManager eventManager;
  ButtonManager buttonManager;
  MotorManager motorManager;
  StateMachine stateMachine;
};
  