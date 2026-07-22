#pragma once
#include <Arduino.h>


/**
 * \ingroup ButtonManager
 * This class detects all button an limit switch presses via ISR triggers.
 * It records presses using event groups which are used by the state maching.
 * */
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
  /// Initalizes all button hit functions as Interrupt Service Routines.
  void init();
private:
};