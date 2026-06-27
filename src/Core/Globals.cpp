//defining all these values in their respective .h files apparently creates a new symbol
// for every .cpp file that includes it. This lead to duplicates of each symbol at link time
// and the compiler didn't like that.

#include "Core/HardwareConfig.h"
#include "Drivers/ButtonManager.h"
#include "Core/TaskManager.h"

volatile uint32_t targetStepPeriod_us = 200;
volatile uint32_t stepPeriod_us = 200;

hw_timer_t *stepTimer = nullptr;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

TaskHandle_t manualModeTask = NULL;
TaskHandle_t calibrationModeTask = NULL;
TaskHandle_t testModeTask = NULL;
TaskHandle_t systemStateSwitcherTask = NULL;
