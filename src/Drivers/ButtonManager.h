#pragma once
#include <Arduino.h>

extern hw_timer_t *stepTimer;
extern portMUX_TYPE timerMux;

class InterruptManager{
  public:
  //apparently ISR handlers must be static
    static void IRAM_ATTR calibrationButtonHit();
    static void IRAM_ATTR limitSwitchHit();
    static void IRAM_ATTR testButtonHit();
    static void IRAM_ATTR cancelButtonHit();
    static void IRAM_ATTR onStepTimer();
    void init();
};