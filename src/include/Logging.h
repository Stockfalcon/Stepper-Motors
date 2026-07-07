#pragma once

#include <Elog.h>

// Define log tags for different modules
#define MAIN_LOG 0
#define MOTOR_LOG 1
#define EVENT_LOG 2
#define STATE_LOG 3
#define TASK_LOG 4
#define BUTTON_LOG 5
#define APP_LOG 6

// Function to initialize logging (call this in setup())
inline void initializeLogging()
{
  Serial.begin(115200);
  Logger.registerSerial(MAIN_LOG, ELOG_LEVEL_VERBOSE, "MAI");
  Logger.registerSerial(MOTOR_LOG, ELOG_LEVEL_DEBUG, "MOT");
  Logger.registerSerial(EVENT_LOG, ELOG_LEVEL_VERBOSE, "EVT");
  Logger.registerSerial(STATE_LOG, ELOG_LEVEL_VERBOSE, "STA");
  Logger.registerSerial(TASK_LOG, ELOG_LEVEL_VERBOSE, "TSK");
  Logger.registerSerial(BUTTON_LOG, ELOG_LEVEL_VERBOSE, "BTN");
  Logger.registerSerial(APP_LOG, ELOG_LEVEL_VERBOSE, "APP");
};