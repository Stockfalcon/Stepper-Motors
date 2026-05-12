#pragma once
#include <Arduino.h>
#include "HardwareConfig.h"
#include "StateMachine.h"

#define EVT_CALIBRATION_BTN (1 << 0)
#define EVT_LIMIT_SWITCH (1 << 1)
#define EVT_CANCEL_BTN (1 << 2)
#define EVT_TEST_BTN (1 << 3)
#define STATE_MANUAL_ACTIVE (1 << 4)
#define STATE_TEST_ACTIVE (1 << 5)
#define STATE_ERROR_ACTIVE (1 << 6)
#define STATE_CALIBRATION_ACTIVE (1 << 7)
EventGroupHandle_t systemEvents = xEventGroupCreate();

typedef struct
{ // Transition table for state switcher
  EventBits_t trigger;
  systemStates fromState;
  systemStates toState;
} stateTransitionRule;

#define NUMBER_OF_STATE_TRANSITIONS 2

stateTransitionRule stateTransitions[NUMBER_OF_STATE_TRANSITIONS] = { // Limit switch & Cancel button logic all in systemStateSwitcher()
    {EVT_CALIBRATION_BTN, MANUAL_MODE, CALIBRATION_MODE},

    {EVT_TEST_BTN, MANUAL_MODE, TEST_MODE}};