#include <Arduino.h>
#include "Application.h"
#include "include/Logging.h"
#include "include/PinMap.h"
#include "include/MotorManager/StepperTest.h"

Application application;
StepperTest st;
int pressed;
int prevPressed;
// When running unit tests we exclude the Arduino sketch entry points
// #ifndef UNIT_TEST
void setup()
{
  delay(3000);
  initializeLogging();

  application.init();
  // Serial.begin(115200);
  // prevPressed = 0;
  // pressed = 0;
  // pinMode(CANCEL_BTN_PIN, INPUT_PULLDOWN);
  // pinMode(CALIBRATION_BTN_PIN, INPUT_PULLDOWN);
  // pinMode(TEST_BTN_PIN, INPUT_PULLDOWN);
  // pinMode(POT_PIN, INPUT);
  // pinMode(2, OUTPUT);


}

void loop()
{
  // *put your main code here, to run repeatedly:
  // if(digitalRead(CALIBRATION_BTN_PIN)){
  //   pressed = !pressed;
  //   digitalWrite(2, pressed);
  // }
  // delay(100);


}
// #endif
