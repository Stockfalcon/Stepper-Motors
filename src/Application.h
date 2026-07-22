#pragma once
#include "include/ButtonManager/ButtonManager.h"
#include "include/MotorManager/MotorManager.h"
#include "include/Communication Structures/EventGroups.h"
#include "StateMachine.h"


/**
 * This class creates all intsances of the classes to run the tensile tester.
 */
class Application{
  public:
  Application() : eventManager(), buttonManager(), motorManager(), stateMachine(motorManager, eventManager){}
  /// The function that starts it all!
  void init();
  private:
  EventManager eventManager;
  ButtonManager buttonManager;
  MotorManager motorManager;
  StateMachine stateMachine;
};
  