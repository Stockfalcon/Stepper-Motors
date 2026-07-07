#include "ButtonManager.h"
#include "include/PinMap.h"
#include "include/Communication Structures/EventGroups.h"
#include "include/Logging.h"


void IRAM_ATTR ButtonManager::calibrationButtonHit()
{
  xEventGroupSetBitsFromISR(EventManager::getInstance().getHandle(), EVT_CALIBRATION_BTN, NULL); // what does the NULL do?
}

void IRAM_ATTR ButtonManager::limitSwitchHit()
{
  xEventGroupSetBitsFromISR(EventManager::getInstance().getHandle(), EVT_LIMIT_SWITCH, NULL); // what does the NULL do?
}

void IRAM_ATTR ButtonManager::testButtonHit()
{
  // timerAlarmDisable(stepTimer);                                                          // ! verify this (maybe try timerStop())
  xEventGroupSetBitsFromISR(EventManager::getInstance().getHandle(), EVT_TEST_BTN, NULL); // what does the NULL do?
}

void IRAM_ATTR ButtonManager::cancelButtonHit()
{
  // timerAlarmDisable(stepTimer);                                                            // ! verify this
  xEventGroupSetBitsFromISR(EventManager::getInstance().getHandle(), EVT_CANCEL_BTN, NULL); // what does the NULL do?
}

void IRAM_ATTR ButtonManager::dummyISR()
{
  void(0);
}

void ButtonManager::init()
{
  Logger.debug(BUTTON_LOG, "Interrupt initialization Started");
  attachInterrupt(CALIBRATION_BTN_PIN, calibrationButtonHit, RISING);
  Logger.debug(BUTTON_LOG, "cal BTN init finnished Started");
  // attachInterrupt(TEST_BTN_PIN, testButtonHit, RISING);
  // attachInterrupt(CANCEL_BTN_PIN, cancelButtonHit, RISING);
  // Logger.debug(BUTTON_LOG, "Interrupt initialization finished");
}