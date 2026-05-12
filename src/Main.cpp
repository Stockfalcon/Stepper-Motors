#include <Arduino.h>
#include "MotorControl.h"
#include "EventGroups.h"
#include "HardwareConfig.h"
#include "InterruptManager.h"
#include "StateMachine.h"
#include "TaskManager.h"


InterruptManager interruptManager;
TaskManager taskManager;

void setup() {
  // *put your setup code here, to run once:
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  Serial.begin(115200);

  interruptManager.init();
  taskManager.init();



}

void loop() {
  // *put your main code here, to run repeatedly:
}



