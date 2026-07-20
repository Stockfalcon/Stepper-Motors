#pragma once
#include <Arduino.h>
#include "include/PinMap.h"
#include "include/Logging.h"

class StepperTest
{
private:
  hw_timer_t *stepTimer = nullptr;
  volatile uint32_t stepPeriod_us = 500;
  volatile uint32_t accumulatedPotVal = 0;
  volatile uint32_t potSampleCounter = 0;
  static portMUX_TYPE timerMux; // = portMUX_INITIALIZER_UNLOCKED
  TaskHandle_t motorControllerTask = NULL;
  static uint32_t stepCount; //

public:
  static void IRAM_ATTR onStepTimer();
  static void castReadPotVal(void *pvParameters);
  void readPotVal();
  void init();

  };