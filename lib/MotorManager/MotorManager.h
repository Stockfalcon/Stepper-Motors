#pragma once
#include <Arduino.h>






class MotorController{
  public:
    void init();
    static void main();
    static void receiveCommands();

    void motorAccelerationControl();
    const uint32_t &getPosition() const;

    static void IRAM_ATTR onStepTimer();

    uint32_t getStepPeriod_us();
    void setStepPeriod_us(uint32_t period_us);
    uint32_t getTargetStepPeriod_us();


  private:
    uint32_t position_cm = 0;
    volatile uint32_t stepPeriod_us = 200;
    volatile uint32_t targetStepPeriod_us = 200;
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
    hw_timer_t *stepTimer = nullptr;


    static bool runMotor;
};