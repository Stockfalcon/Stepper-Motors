#pragma once
#include <Arduino.h>



class ButtonManager
{
public:
  // ButtonManager(eventManager &events) : eventManager(events){}
  // apparently ISR handlers must be static
  static void IRAM_ATTR calibrationButtonHit();
  static void IRAM_ATTR limitSwitchHit();
  static void IRAM_ATTR testButtonHit();
  static void IRAM_ATTR cancelButtonHit();
  static void IRAM_ATTR dummyISR();
  void init();
private:
};