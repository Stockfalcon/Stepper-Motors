#include "InterruptManager.h"
#include "HardwareConfig.h"
#include "EventGroups.h"

void IRAM_ATTR InterruptManager::onStepTimer(){ // linked to hardware timer interupt
  static bool stepState = false;
  stepState = !stepState;
  digitalWrite(STEP_PIN, stepState);
}

void IRAM_ATTR InterruptManager::calibrationButtonHit()
{
  xEventGroupSetBitsFromISR(EventGroups::getInstance().getHandle(), EVT_CALIBRATION_BTN, NULL); // what does the NULL do?
}

void IRAM_ATTR InterruptManager::limitSwitchHit()
{
  xEventGroupSetBitsFromISR(EventGroups::getInstance().getHandle(), EVT_LIMIT_SWITCH, NULL); // what does the NULL do?
}

void IRAM_ATTR InterruptManager::testButtonHit()
{
  timerAlarmDisable(stepTimer);                                       // ! verify this (maybe try timerStop())
  xEventGroupSetBitsFromISR(EventGroups::getInstance().getHandle(), EVT_TEST_BTN, NULL); // what does the NULL do?
}

void IRAM_ATTR InterruptManager::cancelButtonHit()
{
  timerAlarmDisable(stepTimer);                                  // ! verify this
  xEventGroupSetBitsFromISR(EventGroups::getInstance().getHandle(), EVT_CANCEL_BTN, NULL); // what does the NULL do?
}

void InterruptManager::init(){
  stepTimer = timerBegin(
      0,   // timer number (the options are 0,1,2,3)
      80,  // prescale divider (clock is 80 MHz)
      true // count up (true) or down (false)
  );

  timerAttachInterrupt( // when timer is triggered call onStepTimer()
      stepTimer,
      &onStepTimer,
      true // edge triggered timing
  );

  timerAlarmWrite( // write amount of time to trigger alarm to the timer
      stepTimer,
      stepPeriod_us / 2, // divide by two for high AND low
      true);

  timerAlarmEnable(stepTimer); // start the timer
}