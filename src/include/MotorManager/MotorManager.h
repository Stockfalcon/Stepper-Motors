#pragma once
#include <Arduino.h>
#include "include/Globals.h"

struct MotorStates{
  bool runMotor = false;
  bool potEnabled = false;
};


class MotorController : public Task{
  public:
    void init();
    void main() override;

    void motorAccelerationControl();
    const uint32_t &getPosition() const;

    static void IRAM_ATTR onStepTimer();

    uint32_t getStepPeriod_us();
    void setTargetStepPeriod_us(uint32_t period_us);
    uint32_t getTargetStepPeriod_us();


  private:
    
    void receiveCommands();

    void readPotVal();

    uint32_t position_cm = 0;
    volatile uint32_t stepPeriod_us = 200;
    volatile uint32_t targetStepPeriod_us = 200;
    static portMUX_TYPE timerMux; // = portMUX_INITIALIZER_UNLOCKED
    static hw_timer_t *stepTimer; // = nullptr

    MotorStates motorStates;
};