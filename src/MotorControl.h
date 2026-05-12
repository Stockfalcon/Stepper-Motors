#pragma once
#include <Arduino.h>

class MotorController{
  public:
    void setStepperSpeed(uint32_t period_us);
    void motorAccelerationControl();
};