#pragma once
#include "include/ButtonManager/ButtonManager.h"
#include "include/MotorManager/MotorManager.h"
#include "StateMachine.h"

class Application{
  public:
    ButtonManager buttonManager;
    MotorManager motorManager;
    StateMachine stateMachine;
    void init();
  private:
    Application() : buttonManager(), motorManager(), stateMachine(motorManager){}

  };
  