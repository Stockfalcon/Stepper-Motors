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

  /// @brief Calls xEventGroupSetBitsFromISR()
  static void IRAM_ATTR calibrationButtonHit();
  
  /// @brief Calls xEventGroupSetBitsFromISR()
  static void IRAM_ATTR limitSwitchHit();
  /// @brief Calls xEventGroupSetBitsFromISR()
  
  static void IRAM_ATTR testButtonHit();
  
  /// @brief Calls xEventGroupSetBitsFromISR()
  static void IRAM_ATTR cancelButtonHit();
  
  /// @brief Calls xEventGroupSetBitsFromISR()
  static void IRAM_ATTR fwdSwicthSet();
  
  /// @brief Calls xEventGroupSetBitsFromISR()
  static void IRAM_ATTR revSwicthSet();
  
  /// A test ISR.
  static void IRAM_ATTR dummyISR();
  
  /// Initalizes all button hit functions as Interrupt Service Routines.
  void init();
private:
  static MotorManager& motorManager;
};