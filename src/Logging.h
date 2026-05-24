#pragma once

#include <Elog.h>

// Define log tags for different modules
#define MAIN_LOG 0
#define MOTOR_LOG 1
#define EVENT_LOG 2
#define STATE_LOG 3
#define TASK_LOG 4
#define INTERRUPT_LOG 5

// Function to initialize logging (call this in setup())
inline void initializeLogging() {
  Serial.begin(115200);
  Logger.registerSerial(MAIN_LOG, ELOG_LEVEL_DEBUG, "MAI");
  Logger.registerSerial(MOTOR_LOG, ELOG_LEVEL_DEBUG, "MOT");
  Logger.registerSerial(EVENT_LOG, ELOG_LEVEL_DEBUG, "EVT");
  Logger.registerSerial(STATE_LOG, ELOG_LEVEL_DEBUG, "STA");
  Logger.registerSerial(TASK_LOG, ELOG_LEVEL_DEBUG, "TSK");
  Logger.registerSerial(INTERRUPT_LOG, ELOG_LEVEL_DEBUG, "INT");
}

