#pragma once
#include "include/ButtonManager/ButtonManager.h"
#include "include/MotorManager/MotorManager.h"
#include "include/LoadCellManager/FakeLoadCellManager.h"
#include "include/Communication Structures/EventGroups.h"
#include "StateMachine.h"



/// @brief This class creates all intsances of the classes to run the tensile tester and connects them together.
class Application{
  public:
  Application() : eventManager(), buttonManager(), motorManager(), loadCellManager(motorManager), stateMachine(motorManager, eventManager){}
  /// The function that starts it all!
  void init();
  private:
  EventManager eventManager;
  ButtonManager buttonManager;
  MotorManager motorManager;
  FakeLoadCellManager loadCellManager;
  StateMachine stateMachine;
};
  