#pragma once

#include <Arduino.h>
#include "StateMachine.h"

extern TaskHandle_t manualModeTask;
extern TaskHandle_t calibrationModeTask;
extern TaskHandle_t testModeTask;
extern TaskHandle_t systemStateSwitcherTask;


class TaskManager{
  public:
  static void manualMode(void *pvParameter);
  static void calibrationMode(void * pvParameter);
  static void testMode(void *pvParameter);
  void init();
  void deleteStateMachine(StateMachine *pStateMachine);
};

