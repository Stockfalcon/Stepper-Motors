#pragma once
#include "include/ButtonManager/ButtonManager.h"
#include "include/MotorManager/MotorManager.h"
#include "include/LoadCellManager/FakeLoadCellManager.h"
#include "include/Communication Structures/EventGroups.h"
#include "StateMachine.h"



/// @brief This class creates all instances of the classes to run the tensile tester and connects them together.
class Application{
  public:
  Application() : eventManager(), buttonManager(), motorManager(), loadCellManager(motorManager), stateMachine(motorManager, eventManager){}
  /// The function that starts it all!
  void init();
  private:
  EventManager eventManager;///< The EventManager instance.
  ButtonManager buttonManager;///< The ButtonManager instance.
  MotorManager motorManager;///< The MotorManager instance.
  FakeLoadCellManager loadCellManager; ///< The LoadCellManager instance.
  StateMachine stateMachine; ///< The StateMachine instance.
};
  