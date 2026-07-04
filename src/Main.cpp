#include <Arduino.h>
#include "MotorManager/MotorManager.h"
#include "Communication Structures/EventGroups.h"
#include "PinMap.h"
#include <ButtonManager/ButtonManager.h>
#include "StateMachine.h"
#include "Logging.h"

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
