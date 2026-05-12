#include "StateMachine.h"
#include "EventGroups.h"

void StateMachine::onStateEnter(systemStates state)
{
  switch (state)
  {
  case MANUAL_MODE:
    break;

  case CALIBRATION_MODE:
    break;

  case TEST_MODE:
    break;
  }
}

void StateMachine::onStateExit(systemStates state)
{
  switch (state)
  {
  case MANUAL_MODE:
    break;

  case CALIBRATION_MODE:
    break;

  case TEST_MODE:
    break;
  }
}

void StateMachine::systemStateSwitcherTask(void *pvParameters){
  StateMachine *stateMachine = static_cast<StateMachine *>(pvParameters);
  stateMachine -> systemStateSwitcher();
}

void StateMachine::systemStateSwitcher()
{
  for (;;)
  {
    lastState = currentState;
    EventBits_t bits = xEventGroupGetBits(systemEvents);
    if (bits & EVT_LIMIT_SWITCH)
    {
      currentState = MANUAL_MODE;
      xEventGroupClearBits(systemEvents, EVT_LIMIT_SWITCH);
    }
    else if (bits & EVT_CANCEL_BTN)
    {
      currentState = MANUAL_MODE;
      xEventGroupClearBits(systemEvents, EVT_CANCEL_BTN);
    }
    for (int32_t i = 0; i <= NUMBER_OF_STATE_TRANSITIONS; i++)
    {
      if ((bits & stateTransitions[i].trigger) && currentState == stateTransitions[i].fromState)
      {
        currentState = stateTransitions[i].toState;
      }
    }

    if (currentState != lastState)
    {
      onStateEnter(currentState);
      onStateExit(lastState);
    }
  }
}