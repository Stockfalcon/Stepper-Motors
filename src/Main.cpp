#include <Arduino.h>
#include "Application.h"
#include "include/Logging.h"

Application application;

// When running unit tests we exclude the Arduino sketch entry points
// #ifndef UNIT_TEST
void setup()
{
  delay(3000);
  initializeLogging();
  application.init();
}

void loop()
{
  // *put your main code here, to run repeatedly:
  delay(1000);
  Logger.debug(MAIN_LOG, "Running loop");
}
// #endif
