#pragma once
#include <Arduino.h>

class MotorController{
  public:
    void setStepperSpeed(uint32_t period_us);
    void motorAccelerationControl();
    const uint32_t& getPosition() const;
  private:
    uint32_t position_cm = 0;
};