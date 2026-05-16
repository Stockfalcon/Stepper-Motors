#include "MotorControl.h"
#include "HardwareConfig.h"
#include <Arduino.h>
#include "InterruptManager.h"


void MotorController::setStepperSpeed(uint32_t period_us)
{                                // call from any core or task (should only really be called by motorControl)
  portENTER_CRITICAL(&timerMux); // temporairily disable interrupt and prevent ISR from running mid update
  stepPeriod_us = period_us;
  timerAlarmWrite(stepTimer, period_us / 2, true); // update timer alarm period
  portEXIT_CRITICAL(&timerMux);
}

void MotorController::motorAccelerationControl()
{ // pinned to core 0 (core 0's only task)
  for (;;)
  {
    // Read target safely
    portENTER_CRITICAL(&timerMux);
    uint32_t target = targetStepPeriod_us;
    uint32_t current = stepPeriod_us;
    portEXIT_CRITICAL(&timerMux);

    if (target < current)
    {   // decrease speed
      setStepperSpeed(current - stepAccel);
      // Re-check for overshoot
      portENTER_CRITICAL(&timerMux);
      if (target > stepPeriod_us)
      {
        setStepperSpeed(target);
      }
      portEXIT_CRITICAL(&timerMux);
    }
    else if (target > current)
    { // increase speed
      setStepperSpeed(current + stepAccel);
      // Re-check for overshoot
      portENTER_CRITICAL(&timerMux);
      if (target > stepPeriod_us)
      {
        setStepperSpeed(target);
      }
      portEXIT_CRITICAL(&timerMux);
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

const uint32_t& MotorController::getPosition() const{
  return position_cm;
}