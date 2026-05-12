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
    if (targetStepPeriod_us < stepPeriod_us)
    { // decrease speed
      setStepperSpeed(stepPeriod_us -= stepAccel);
      if (targetStepPeriod_us > stepPeriod_us)
      { // detect overshoot
        setStepperSpeed(targetStepPeriod_us);
      }
    }
    else if (targetStepPeriod_us > stepPeriod_us)
    { // increase speed
      setStepperSpeed(stepPeriod_us += stepAccel);
      if (targetStepPeriod_us > stepPeriod_us)
      { // detect overshoot
        setStepperSpeed(targetStepPeriod_us);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
