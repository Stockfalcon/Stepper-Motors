#include "StepperTest.h"

portMUX_TYPE StepperTest::timerMux = portMUX_INITIALIZER_UNLOCKED;
uint32_t StepperTest::stepCount = 0;

void StepperTest::onStepTimer(){
  static bool stepState = false;
  stepState = !stepState;
  stepCount++;
  digitalWrite(STEP_PIN, stepState);
  digitalWrite(STEP_PIN, stepState);
  if (stepCount % 101 == 0)
  {
    digitalWrite(2, stepState);
  }
}


void StepperTest::readPotVal(){
  Logger.verbose(MOTOR_LOG, "reading pot val");
  accumulatedPotVal += analogRead(POT_PIN);
  potSampleCounter++;

  if (potSampleCounter >= 10)
  {
    uint32_t avgPotVal = accumulatedPotVal / potSampleCounter;
    accumulatedPotVal = 0;
    potSampleCounter = 0;
    uint32_t period_us = map(avgPotVal, 0, 4095, 1000, 200);
    Logger.trace(MOTOR_LOG, "setting step period to %lu us", (unsigned long)period_us);
    Logger.trace(MOTOR_LOG, "Setting step period");
    portENTER_CRITICAL(&timerMux);
    stepPeriod_us = period_us;
    portEXIT_CRITICAL(&timerMux);
  }
}

void StepperTest::castReadPotVal(void* pvParameters){
  if (pvParameters == nullptr)
  {
    vTaskDelete(nullptr);
    return;
  }

  StepperTest* self = static_cast<StepperTest*>(pvParameters);
  for (;;)
  {
    self->readPotVal();
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

void StepperTest::init(){
  pinMode(STEP_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(EN_PIN, 0);

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


  //! Currently doesn't work
  BaseType_t result = xTaskCreatePinnedToCore(
      StepperTest::castReadPotVal,
      "motorTask",
      10000,
      this,
      1,
      &motorControllerTask,
      1);
}

