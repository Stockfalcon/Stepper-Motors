#pragma once
#include <Arduino.h>

#define EVT_CALIBRATION_BTN (1 << 0)
#define EVT_LIMIT_SWITCH (1 << 1)
#define EVT_CANCEL_BTN (1 << 2)
#define EVT_TEST_BTN (1 << 3)
#define STATE_MANUAL_ACTIVE (1 << 4)
#define STATE_TEST_ACTIVE (1 << 5)
#define STATE_ERROR_ACTIVE (1 << 6)
#define STATE_CALIBRATION_ACTIVE (1 << 7)

enum systemStates
{
  MANUAL_MODE,
  CALIBRATION_MODE,
  TEST_MODE,
  ERROR
};

class StateManager
{
private:
  EventGroupHandle_t systemEvents = xEventGroupCreate();

  typedef struct
  { // Transition table for state switcher
    EventBits_t trigger;
    systemStates fromState;
    systemStates toState;
  } stateTransitionRule;

  static const u_int32_t numberOfStateTransitions = 2;
  stateTransitionRule stateTransitions[numberOfStateTransitions] = {// Limit switch & Cancel button logic all in main()
      {EVT_CALIBRATION_BTN, MANUAL_MODE, CALIBRATION_MODE},
      {EVT_TEST_BTN, MANUAL_MODE, TEST_MODE}};

public:
  EventGroupHandle_t getHandle();

  static StateManager &getInstance();

  uint32_t getNumberOfStateTransitions() const;
  const stateTransitionRule &getStateTransitions(uint32_t index) const;
};