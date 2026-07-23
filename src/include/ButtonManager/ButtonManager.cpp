#include "ButtonManager.h"
#include "include/PinMap.h"
#include "include/Communication Structures/EventGroups.h"
#include "include/Logging.h"
#include "include/Communication Structures/Queues.h"
#include "include/MotorManager/MotorManager.h"

void IRAM_ATTR ButtonManager::calibrationButtonHit()
{
  xEventGroupSetBitsFromISR(EventManager::getHandleFromISR(), EVT_CALIBRATION_BTN, NULL); // what does the NULL do?
}

void IRAM_ATTR ButtonManager::limitSwitchHit()
{
  xEventGroupSetBitsFromISR(EventManager::getHandleFromISR(), EVT_LIMIT_SWITCH, nullptr); // what does the NULL do?
}

void IRAM_ATTR ButtonManager::testButtonHit()
{
  // timerAlarmDisable(stepTimer);                                                          // ! verify this (maybe try timerStop())
  xEventGroupSetBitsFromISR(EventManager::getHandleFromISR(), EVT_TEST_BTN, NULL); // what does the NULL do?
}

void IRAM_ATTR ButtonManager::cancelButtonHit()
{
  // timerAlarmDisable(stepTimer);                                                            // ! verify this
  xEventGroupSetBitsFromISR(EventManager::getHandleFromISR(), EVT_CANCEL_BTN, NULL); // what does the NULL do?
}

void IRAM_ATTR ButtonManager::fwdSwicthSet(){
  MotorCommand command ={
    .type = CHANGE_DIR
  };
  xQueueSendFromISR(motorCommandQueue,(MotorCommand *)&command, NULL);
}

void IRAM_ATTR ButtonManager::revSwicthSet(){
  MotorCommand command ={
    .type = CHANGE_DIR
  };
  xQueueSendFromISR(motorCommandQueue,(MotorCommand *)&command, NULL);
}


void IRAM_ATTR ButtonManager::dummyISR()
{
  void(0);
}

void ButtonManager::init()
{
  Logger.debug(BUTTON_LOG, "Interrupt initialization Started");
  pinMode(CANCEL_BTN_PIN, INPUT_PULLDOWN);
  pinMode(CALIBRATION_BTN_PIN, INPUT_PULLDOWN);
  pinMode(TEST_BTN_PIN, INPUT_PULLDOWN);
  pinMode(FWD_SWITCH_PIN, INPUT_PULLDOWN);
  pinMode(REV_SWITCH_PIN, INPUT_PULLDOWN);
  pinMode(POT_PIN, INPUT);
  pinMode(2, OUTPUT);
  attachInterrupt(CALIBRATION_BTN_PIN, calibrationButtonHit, RISING);
  attachInterrupt(TEST_BTN_PIN, testButtonHit, RISING);
  attachInterrupt(CANCEL_BTN_PIN, cancelButtonHit, RISING);
  attachInterrupt(FWD_SWITCH_PIN, cancelButtonHit, RISING);
  attachInterrupt(REV_SWITCH_PIN, cancelButtonHit, RISING);
  Logger.debug(BUTTON_LOG, "Interrupt initialization finished");
}