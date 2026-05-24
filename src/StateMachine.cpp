#include "StateMachine.h"
#include "EventGroups.h"
#include "Logging.h"

void StateMachine::onStateEnter(systemStates state)
{
  switch (state)
  {
  case MANUAL_MODE:
  xEventGroupSetBits(EventGroups::getInstance().getHandle(), STATE_MANUAL_ACTIVE);
  Logger.debug(STATE_LOG, "STATE_MANUAL_ACTIVE set");
  break;
  
  case CALIBRATION_MODE:
    xEventGroupSetBits(EventGroups::getInstance().getHandle(), STATE_CALIBRATION_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_CALIBRATION_ACTIVE set");
    break;
    
    case TEST_MODE:
    xEventGroupSetBits(EventGroups::getInstance().getHandle(), STATE_CALIBRATION_ACTIVE);
    break;
  }
}

void StateMachine::onStateExit(systemStates state)
{
  switch (state)
  {
    case MANUAL_MODE:
    xEventGroupClearBits(EventGroups::getInstance().getHandle(), STATE_MANUAL_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_MANUAL_ACTIVE reset");
    break;
    
    case CALIBRATION_MODE:
    xEventGroupClearBits(EventGroups::getInstance().getHandle(), STATE_CALIBRATION_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_CALIBRATION_ACTIVE reset");
    break;
    
    case TEST_MODE:
    xEventGroupClearBits(EventGroups::getInstance().getHandle(), STATE_TEST_ACTIVE);
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
    EventBits_t bits = xEventGroupGetBits(EventGroups::getInstance().getHandle());
    if (bits & EVT_LIMIT_SWITCH)
    {
      Logger.debug(STATE_LOG, "Limit Switch hit detected");
      currentState = MANUAL_MODE;
      xEventGroupClearBits(EventGroups::getInstance().getHandle(), EVT_LIMIT_SWITCH);
    }
    else if (bits & EVT_CANCEL_BTN)
    {
      Logger.debug(STATE_LOG, "Cancel Button detected");
      Logger.verbose(STATE_LOG, "systemEvents before clear bits: %d", bits);
      currentState = MANUAL_MODE;
      xEventGroupClearBits(EventGroups::getInstance().getHandle(), EVT_CANCEL_BTN);
      Logger.verbose(STATE_LOG, "systemEvents after clear bits: %d", xEventGroupGetBits(EventGroups::getInstance().getHandle()));
    }
    for (int32_t i = 0; i <= EventGroups::getInstance().getNumberOfStateTransitions(); i++)
    {
      auto stateTransitions = EventGroups::getInstance().getStateTransitions(i);
      if ((bits & stateTransitions.trigger) && currentState == stateTransitions.fromState)
      {
        currentState = stateTransitions.toState;
      }
    }

    if (currentState != lastState)
    {
      onStateEnter(currentState);
      onStateExit(lastState);
    }
  }
}