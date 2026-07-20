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

    static void motorAccelerationControl(void* pvParameters);
    const uint32_t &getPosition() const;
    static void IRAM_ATTR onStepTimer();
    void clearStepCount();

    uint32_t getSteps();
    static void setTargetStepPeriod_us(uint32_t period_us);
    static uint32_t getTargetStepPeriod_us();
    static void setStepPeriod_us(uint32_t period_us);
    static uint32_t getStepPeriod_us();

    void sendToQueue(const MotorCommand &command);

  private:
    
    void receiveCommands();

    void readPotVal();

    uint32_t position_cm = 0;
    static volatile uint32_t stepPeriod_us;
    static volatile uint32_t targetStepPeriod_us;
    static portMUX_TYPE timerMux; // = portMUX_INITIALIZER_UNLOCKED
    static portMUX_TYPE stepMux; // = portMUX_INITIALIZER_UNLOCKED
    static hw_timer_t *stepTimer;
    static uint32_t stepCount;// could use inline keyword
    volatile uint32_t potSampleCounter = 0;
    volatile uint32_t accumulatedPotVal = 0;

    MotorStates motorStates;
    TaskHandle_t motorControllerTask = NULL;
    TaskHandle_t motorAccelerationTask = NULL;
    QueueHandle_t MotorCommandQueue = nullptr;
};