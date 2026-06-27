#pragma once
#include <Arduino.h>


#define DIR_PIN 18
#define EN_PIN 4
#define STEP_PIN 19
#define pass (void)0
#define stepAccel 10 // steps us^-2

#define CALIBRATION_BTN_PIN 27
#define TEST_BTN_PIN 26
#define CANCEL_BTN_PIN 25
#define FWD_SWITCH_PIN 12
#define REV_SWITCH_PIN 14
#define POT_PIN 33

#define MAIN_LOG 0

extern volatile uint32_t targetStepPeriod_us;
extern volatile uint32_t stepPeriod_us; // initial speed