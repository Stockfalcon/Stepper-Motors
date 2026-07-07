#pragma once
#include <Arduino.h>
#include "include/Globals.h"

enum MotorCommandType
{
  RUN,
  STOP,
  ENABLE_LIMIT_SWITCH_ALERT
};

struct MotorCommand
{
  MotorCommandType type;
};

struct MotorStates{
  bool runMotor = true;
  bool potEnabled = true;
};


class MotorManager : public Task{
  public:
    void init();
    void main() override;

    void motorAccelerationControl();
    const uint32_t &getPosition() const;

    static void IRAM_ATTR onStepTimer();

    uint32_t getStepPeriod_us();
    void setTargetStepPeriod_us(uint32_t period_us);
    uint32_t getTargetStepPeriod_us();
    void setStepPeriod_us(uint32_t period_us);

    void sendToQueue(const MotorCommand *command);

  private:
    
    void receiveCommands();

    void readPotVal();

    uint32_t position_cm = 0;
    volatile uint32_t stepPeriod_us = 2000;
    volatile uint32_t targetStepPeriod_us = 200;
    static portMUX_TYPE timerMux; // = portMUX_INITIALIZER_UNLOCKED
    hw_timer_t *stepTimer = nullptr;

    MotorStates motorStates;
    TaskHandle_t motorControllerTask = NULL;
    QueueHandle_t MotorCommandQueue = nullptr;
};