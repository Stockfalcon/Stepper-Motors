#pragma once

#include <Arduino.h>

extern TaskHandle_t manualModeTask = NULL;
extern TaskHandle_t calibrationModeTask = NULL;
extern TaskHandle_t testModeTask = NULL;
extern TaskHandle_t systemStateSwitcherTask = NULL;


class TaskManager{
  public:
  static void manualMode(void *pvParameter);
  static void calibrationMode(void * pvParameter);
  static void testMode(void *pvParameter);
  void init();
  void deleteStateMachine(StateMachine *pStateMachine);
};

