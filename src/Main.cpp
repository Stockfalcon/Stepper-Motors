#include <Arduino.h>
#include "include/MotorManager/MotorManager.h"
#include "include/Communication Structures/EventGroups.h"
#include "include/PinMap.h"
#include "include/ButtonManager/ButtonManager.h"
#include "StateMachine.h"
#include "include/Logging.h"

ButtonManager buttonManager;

// When running unit tests we exclude the Arduino sketch entry points
#ifndef UNIT_TEST
void setup()
{
  delay(3000);
  initializeLogging();

  // *put your setup code here, to run once:
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  buttonManager.init();

  Logger.debug(MAIN_LOG, "Initializations complete");
}

void loop()
{
  // *put your main code here, to run repeatedly:
  delay(1000);
  Logger.debug(MAIN_LOG, "Running loop");
}
#endif
