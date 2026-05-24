#include "TaskManager.h"
#include "StateMachine.h"
#include "EventGroups.h"
#include "InterruptManager.h"

void TaskManager::manualMode(void *pvParameters)
{
  uint32_t accumulatedPotVal = 0;
  int counter = 0;
  for (;;)
  {
    xEventGroupWaitBits(                        // Puts to sleep until correct bits are activated
        EventGroups::getInstance().getHandle(), // Event group handle
        STATE_MANUAL_ACTIVE,                    // Bit to wait for. Use EVT_TEST_ACTIVE | EVT_FAULT for multiple conditions
        pdFALSE,                                // Don't clear bits!
        pdTRUE,                                 // Wait for all bits
        portMAX_DELAY                           // wait forever
    );
    while (xEventGroupGetBits(EventGroups::getInstance().getHandle()) && STATE_MANUAL_ACTIVE)
    { // Keeps the task active until Status flag cleared
      counter++;
      accumulatedPotVal += analogRead(POT_PIN);
      if (counter == 10)
      {
        uint32_t avgPotVal = accumulatedPotVal / 10;
        accumulatedPotVal = 0;
        counter = 0;
        uint32_t period_us = map(avgPotVal, 0, 4095, 1000, 200);
        portENTER_CRITICAL(&timerMux);
        targetStepPeriod_us = period_us;
        portEXIT_CRITICAL(&timerMux);
        Serial.print(period_us);
        Serial.print("  Core");
        Serial.println(xPortGetCoreID());
      }
      vTaskDelay(pdMS_TO_TICKS(1)); // run at ~1kHz
    }
  }
}

void TaskManager::calibrationMode(void *pvParameter)
{ 
  for(;;){
    xEventGroupWaitBits(
      EventGroups::getInstance().getHandle(),
      STATE_CALIBRATION_ACTIVE,
      pdFALSE,
      pdTRUE,
      portMAX_DELAY
    );

    while (xEventGroupGetBits(EventGroups::getInstance().getHandle()) && STATE_CALIBRATION_ACTIVE)
    {
      vTaskDelay(pdMS_TO_TICKS(1));
    }
  }
}

void TaskManager::testMode(void *pvParameter)
{
  xEventGroupWaitBits(
      EventGroups::getInstance().getHandle(),
      STATE_TEST_ACTIVE,
      pdFALSE,
      pdTRUE,
      portMAX_DELAY);

  while (xEventGroupGetBits(EventGroups::getInstance().getHandle()) && STATE_TEST_ACTIVE)
  {
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

void TaskManager::init()
{
  StateMachine *stateMachine = new StateMachine();
  xTaskCreatePinnedToCore(
      StateMachine::systemStateSwitcherTask,
      "systemStateSwitcher",
      10000,                    // usStackDepth
      stateMachine,             // Parameters
      1,                        // Priority
      &systemStateSwitcherTask, // handle
      1                         // Core ID
  );

  xTaskCreatePinnedToCore(
    TaskManager::manualMode,
    "manualMode",
    10000,
    NULL,
    1,
    &manualModeTask,
    1
  );

  xTaskCreatePinnedToCore(
    TaskManager::calibrationMode,
    "manualMode",
    10000,
    NULL,
    1,
    &calibrationModeTask,
    1
  );
  
  xTaskCreatePinnedToCore(
    TaskManager::testMode,
    "calibrationMode",
    10000,
    NULL,
    1,
    &testModeTask,
    1
  );

}

void TaskManager::deleteStateMachine(StateMachine *pStateMachine)
{
  delete pStateMachine;
  pStateMachine = NULL;
}