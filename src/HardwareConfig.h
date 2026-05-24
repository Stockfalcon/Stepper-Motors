#pragma once
#include <Arduino.h>


#define DIR_PIN 18
#define EN_PIN 0
#define STEP_PIN 19
#define POT_PIN 25
#define pass (void)0
#define stepAccel 10 // steps us^-2

extern volatile uint32_t targetStepPeriod_us;
extern volatile uint32_t stepPeriod_us; // initial speed