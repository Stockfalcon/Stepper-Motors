#include <Arduino.h>
#include "MotorControl.h"
#include "EventGroups.h"
#include "HardwareConfig.h"
#include "InterruptManager.h"
#include "StateMachine.h"
#include "TaskManager.h"
#include "Logging.h"

InterruptManager interruptManager;
TaskManager taskManager;


// When running unit tests we exclude the Arduino sketch entry points
#ifndef UNIT_TEST
void setup() {
  delay(3000);
  initializeLogging();

  // *put your setup code here, to run once:
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);


  interruptManager.init();
  taskManager.init();

  Logger.debug(MAIN_LOG, "Initializations complete");


}

void loop() {
  // *put your main code here, to run repeatedly:
  delay(1000);
  Logger.debug(MAIN_LOG, "Running loop");
}
#endif



