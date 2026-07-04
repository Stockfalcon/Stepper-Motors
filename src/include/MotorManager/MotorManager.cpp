#include "MotorManager.h"
#include "PinMap.h"
#include <Arduino.h>
#include "Logging.h"
#include "Globals.h"
#include "Communication Structures/Queues.h"


void MotorController::main()
{
receiveCommands();
if (motorStates.potEnabled){
  readPotVal();
}
}

void MotorController::receiveCommands()
{
  MotorCommand message;
  if(xQueueReceive(MotorCommandQueue, &message, 0) != pdTRUE && uxQueueMessagesWaiting(MotorCommandQueue) != 0){
    Logger.warning(MOTOR_LOG,"Motor Controller failed to receive command from queue.");
  }
  switch (message.type){
    case(RUN):
    timerAlarmEnable(stepTimer);
    break;
    case(STOP):
    timerAlarmDisable(stepTimer);
    xQueueReset(MotorCommandQueue);
    break;
  }
}

void MotorController::motorAccelerationControl()
{ // pinned to core 0 (core 0's only task)
  for (;;)
  {
    uint32_t target = getTargetStepPeriod_us();
    uint32_t current = getStepPeriod_us();

    if (target < current)
    { // decrease speed
      setTargetStepPeriod_us(current - stepAccel);
      // Re-check for overshoot
      if (target > current)
      {
        setTargetStepPeriod_us(target);
      }
      Logger.debug(MOTOR_LOG, "Decreased speed");
    }
    else if (target > current)
    { // increase speed
      setTargetStepPeriod_us(current + stepAccel);
      if (target > current)
      {
        setTargetStepPeriod_us(target);
      }
      Logger.debug(MOTOR_LOG, "Increased speed");
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  };
}

const uint32_t &MotorController::getPosition() const
{
  Logger.debug(MOTOR_LOG, "Returned position");
  return position_cm;
}

void MotorController::init(){
  MotorController motorController;
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);

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

  xTaskCreatePinnedToCore(
    Task::taskEntry,
    "motorTask",
    10000, 
    &motorController,
    0,
    &motorControllerTask,
    1
  );
}

void IRAM_ATTR MotorController::onStepTimer()
{ // linked to hardware timer interupt
  static bool stepState = false;
  stepState = !stepState;
  digitalWrite(STEP_PIN, stepState);
}

uint32_t MotorController::getStepPeriod_us()
{
  // temporairily disable interrupt and prevent ISR from running mid update
  portENTER_CRITICAL(&timerMux);
  uint32_t period_us = stepPeriod_us;
  portEXIT_CRITICAL(&timerMux);
  return period_us;
}

uint32_t MotorController::getTargetStepPeriod_us()
{
  // temporairily disable interrupt and prevent ISR from running mid update
  portENTER_CRITICAL(&timerMux);
  uint32_t period_us = targetStepPeriod_us;
  portEXIT_CRITICAL(&timerMux);
  return period_us;
}

void MotorController::setTargetStepPeriod_us(uint32_t period_us)
{
  // temporairily disable interrupt and prevent ISR from running mid update
  portENTER_CRITICAL(&timerMux);
  targetStepPeriod_us = period_us;
  timerAlarmWrite(stepTimer, period_us / 2, true); // update timer alarm period
  portEXIT_CRITICAL(&timerMux);
}

void MotorController::readPotVal()
{
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
    setTargetStepPeriod_us(period_us);
  }
}

void MotorController::sendToQueue(const MotorCommand *command){
  xQueueSendToBack(MotorCommandQueue, command, 0);
}