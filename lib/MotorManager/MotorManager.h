#pragma once
#include <Arduino.h>



class SpeedController
{
public:
  SpeedController(MotorController& motor) : motorController(motor)
  {}
  void motorAccelerationControl();
  const uint32_t &getPosition() const;

private:
  uint32_t position_cm = 0;
  MotorController& motorController;
};


class MotorController{
  public:
    static void IRAM_ATTR onStepTimer();
    uint32_t getStepPeriod_us();
    uint32_t getTargetStepPeriod_us();
    void setStepPeriod_us(uint32_t period_us);
    void init();

  private:
    volatile uint32_t stepPeriod_us = 200;
    volatile uint32_t targetStepPeriod_us = 200;
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
    hw_timer_t *stepTimer = nullptr;
};