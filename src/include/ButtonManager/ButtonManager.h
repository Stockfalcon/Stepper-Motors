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
  /// Calls xEventGroupSetBitsFromISR()
  static void IRAM_ATTR calibrationButtonHit();
  /// Calls xEventGroupSetBitsFromISR()
  static void IRAM_ATTR limitSwitchHit();
  /// Calls xEventGroupSetBitsFromISR()
  static void IRAM_ATTR testButtonHit();
  /// Calls xEventGroupSetBitsFromISR()
  static void IRAM_ATTR cancelButtonHit();
  /// A test ISR.
  static void IRAM_ATTR dummyISR();
  /// Initalizes all button hit functions as Interrupt Service Routines.
  void init();
private:
};