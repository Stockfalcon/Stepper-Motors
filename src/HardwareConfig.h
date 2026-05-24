#pragma once
#include <Arduino.h>


#define DIR_PIN 18
#define EN_PIN 0
#define STEP_PIN 19
#define POT_PIN 25
#define pass (void)0
#define stepAccel 10 // steps us^-2

#define CALIBRATION_BTN_PIN 1 
#define TEST_BTN_PIN 1
#define CANCEL_BTN_PIN 1

#define MAIN_LOG 0

extern volatile uint32_t targetStepPeriod_us;
extern volatile uint32_t stepPeriod_us; // initial speed