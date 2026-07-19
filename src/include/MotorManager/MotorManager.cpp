#include "MotorManager.h"
#include "PinMap.h"
#include <Arduino.h>
#include "Logging.h"
#include "Globals.h"
#include "Communication Structures/Queues.h"

uint32_t MotorManager::stepCount = 0;

void MotorManager::init()
{
  // pinMode(EN_PIN, OUTPUT);
  // digitalWrite(EN_PIN, LOW);
  
  MotorCommandQueue = xQueueCreate(10, sizeof(MotorCommand));
  if (MotorCommandQueue == nullptr)
  {
    Logger.error(MOTOR_LOG, "Motor command queue creation failed");
    return;
  }

  Logger.trace(MOTOR_LOG, "Motor timer alarm initialization started");
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

  BaseType_t result = xTaskCreatePinnedToCore(
        Task::taskEntry,
        "motorTask",
        10000,
        this,
        1,
        &motorControllerTask,
        1);
  printf("\n\nresult: %d\n\n", result);
  Logger.info(MOTOR_LOG, "Motor initializations Finnished.");
}

void MotorManager::main()
{
  for(;;){
    receiveCommands();
    if (motorStates.potEnabled){
      readPotVal();
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void MotorManager::receiveCommands()
{
  MotorCommand message{};
  if(xQueueReceive(MotorCommandQueue, &message, pdMS_TO_TICKS(100)) == pdTRUE ){
    switch (message.type){
      case(RUN):
      timerAlarmEnable(stepTimer);
      Logger.info(MOTOR_LOG, "Recieved run!");
      motorStates.potEnabled = true;
      break;
      case(STOP):
      timerAlarmDisable(stepTimer);
      Logger.info(MOTOR_LOG, "Recieved stop!");
      motorStates.potEnabled = false;
      xQueueReset(MotorCommandQueue);
      break;
    }
  }
  else{
  Logger.debug(MOTOR_LOG,"Motor Controller failed to receive command from queue.");
  }
}

void MotorManager::motorAccelerationControl()
{ // pinned to core 0 (core 0's only task)
  for (;;)
  {
    uint32_t target = getTargetStepPeriod_us();
    uint32_t current = getStepPeriod_us();

    if (target < current)
    { // decrease speed
      setStepPeriod_us(current - stepAccel);
      // Re-check for overshoot
      if (target > current)
      {
        setStepPeriod_us(target);
      }
      Logger.debug(MOTOR_LOG, "Decreased speed");
    }
    else if (target > current)
    { // increase speed
      setStepPeriod_us(current + stepAccel);
      if (target > current)
      {
        setStepPeriod_us(target);
      }
      Logger.debug(MOTOR_LOG, "Increased speed");
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  };
}

const uint32_t &MotorManager::getPosition() const
{
  Logger.debug(MOTOR_LOG, "Returned position");
  return position_cm;
}

void IRAM_ATTR MotorManager::onStepTimer()
{ // linked to hardware timer interupt
  static bool stepState = false;
  stepState = !stepState;
  stepCount ++;
  digitalWrite(STEP_PIN, stepState);
}

uint32_t MotorManager::getStepPeriod_us()
{
  // temporairily disable interrupt and prevent ISR from running mid update
  portENTER_CRITICAL(&timerMux);
  uint32_t period_us = stepPeriod_us;
  portEXIT_CRITICAL(&timerMux);
  return period_us;
}
uint32_t MotorManager::getSteps()
{
  // temporairily disable interrupt and prevent ISR from running mid update
  portENTER_CRITICAL(&stepMux);
  uint32_t steps = stepCount;
  portEXIT_CRITICAL(&stepMux);
  return steps;
}

uint32_t MotorManager::getTargetStepPeriod_us()
{
  // temporairily disable interrupt and prevent ISR from running mid update
  portENTER_CRITICAL(&timerMux);
  uint32_t period_us = targetStepPeriod_us;
  portEXIT_CRITICAL(&timerMux);
  return period_us;
}

void MotorManager::setTargetStepPeriod_us(uint32_t period_us)
{
  // temporairily disable interrupt and prevent ISR from running mid update
  Logger.trace(MOTOR_LOG, "Setting target step period");
  portENTER_CRITICAL(&timerMux);
  targetStepPeriod_us = period_us;
  portEXIT_CRITICAL(&timerMux);
}

void MotorManager::setStepPeriod_us(uint32_t period_us)
{
  // temporairily disable interrupt and prevent ISR from running mid update
  Logger.trace(MOTOR_LOG, "Setting step period");
  portENTER_CRITICAL(&timerMux);
  stepPeriod_us = period_us;
  timerAlarmWrite(stepTimer, period_us/2, true);
  portEXIT_CRITICAL(&timerMux);
}

void MotorManager::readPotVal()
{
  Logger.verbose(MOTOR_LOG, "reading pot val");
  uint32_t accumulatedPotVal = 0;
  int counter = 0;
  counter++;
  accumulatedPotVal += analogRead(POT_PIN);
  if (counter == 10)
  {
    uint32_t avgPotVal = accumulatedPotVal / 10;
    accumulatedPotVal = 0;
    counter = 0;
    uint32_t period_us = map(avgPotVal, 0, 4095, 1000, 200);
    Logger.trace(MOTOR_LOG, "setting target step period to %2f us", period_us);
    setTargetStepPeriod_us(period_us);
  }
}

void MotorManager::sendToQueue(const MotorCommand &command){
  if (MotorCommandQueue == nullptr)
  {
    Logger.error(MOTOR_LOG, "Motor command queue is not initialized");
    MotorCommandQueue = xQueueCreate(10, sizeof(MotorCommand));
    if (MotorCommandQueue == nullptr)
    {
      Logger.error(MOTOR_LOG, "Motor command queue creation failed");
      return;
    }
    Logger.info(MOTOR_LOG, "Motor command queue created");
    return;
  }

  if (xQueueSendToBack(MotorCommandQueue, &command, pdMS_TO_TICKS(100)) != pdTRUE)
  {
    Logger.warning(MOTOR_LOG, "Motor command queue send timed out");
  }
}

void MotorManager::clearStepCount(){
  stepCount = 0;
}