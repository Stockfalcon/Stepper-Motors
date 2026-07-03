#include "ButtonManager.h"
#include "pinMap.h"
#include "Communication Structures/EventGroups.h"
#include "Logging.h"


void IRAM_ATTR ButtonManager::calibrationButtonHit()
{
  xEventGroupSetBitsFromISR(StateManager::getInstance().getHandle(), EVT_CALIBRATION_BTN, NULL); // what does the NULL do?
}

void IRAM_ATTR ButtonManager::limitSwitchHit()
{
  xEventGroupSetBitsFromISR(StateManager::getInstance().getHandle(), EVT_LIMIT_SWITCH, NULL); // what does the NULL do?
}

void IRAM_ATTR ButtonManager::testButtonHit()
{
  // timerAlarmDisable(stepTimer);                                                          // ! verify this (maybe try timerStop())
  xEventGroupSetBitsFromISR(StateManager::getInstance().getHandle(), EVT_TEST_BTN, NULL); // what does the NULL do?
}

void IRAM_ATTR ButtonManager::cancelButtonHit()
{
  // timerAlarmDisable(stepTimer);                                                            // ! verify this
  xEventGroupSetBitsFromISR(StateManager::getInstance().getHandle(), EVT_CANCEL_BTN, NULL); // what does the NULL do?
}

void ButtonManager::init()
{
  attachInterrupt(CALIBRATION_BTN_PIN, calibrationButtonHit, RISING);
  attachInterrupt(TEST_BTN_PIN, testButtonHit, RISING);
  attachInterrupt(CANCEL_BTN_PIN, cancelButtonHit, RISING);
  Logger.debug(BUTTON_LOG, "Interrupt initialization finished");
}