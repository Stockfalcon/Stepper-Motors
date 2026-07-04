#include "StateMachine.h"
#include "include/Communication Structures/EventGroups.h"
#include "include/Communication Structures/Queues.h"
#include "include/Logging.h"

void StateMachine::onStateEnter(systemStates state)
{
  switch (state)
  {
  case MANUAL_MODE:
  {
    xEventGroupSetBits(StateManager::getInstance().getHandle(), STATE_MANUAL_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_MANUAL_ACTIVE set");
    MotorCommand motorCommand{
        .type = RUN};
    motorController.sendToQueue((MotorCommand*)&motorCommand);
    break;
  }

  case CALIBRATION_MODE:
  {
    xEventGroupSetBits(StateManager::getInstance().getHandle(), STATE_CALIBRATION_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_CALIBRATION_ACTIVE set");
    break;
  }

  case TEST_MODE:
  {
    xEventGroupSetBits(StateManager::getInstance().getHandle(), STATE_CALIBRATION_ACTIVE);
    break;
  }
  }
}

void StateMachine::onStateExit(systemStates state)
{
  switch (state)
  {
  case MANUAL_MODE:
  {
    xEventGroupClearBits(StateManager::getInstance().getHandle(), STATE_MANUAL_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_MANUAL_ACTIVE reset");
    MotorCommand motorCommand{
      .type = STOP
    };
    motorController.sendToQueue((MotorCommand *)&motorCommand);
    break;
  }

  case CALIBRATION_MODE:
  {
    xEventGroupClearBits(StateManager::getInstance().getHandle(), STATE_CALIBRATION_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_CALIBRATION_ACTIVE reset");
    break;
  }

  case TEST_MODE:
  {
    xEventGroupClearBits(StateManager::getInstance().getHandle(), STATE_TEST_ACTIVE);
    break;
  }
  }
}


void StateMachine::main()
{
  for (;;)
  {
    lastState = currentState;
    EventBits_t bits = xEventGroupGetBits(StateManager::getInstance().getHandle());
    for (int32_t i = 0; i <= StateManager::getInstance().getNumberOfStateTransitions(); i++)
    {
      auto stateTransitions = StateManager::getInstance().getStateTransitions(i);
      if ((bits & stateTransitions.trigger) && currentState == stateTransitions.fromState)
      {
        currentState = stateTransitions.toState;
      }
    }
    if (bits & EVT_LIMIT_SWITCH)
    {
      Logger.debug(STATE_LOG, "Limit Switch hit detected");
      Logger.verbose(STATE_LOG, "systemEvents before clear bits: %d", bits);
      currentState = MANUAL_MODE;
      xEventGroupClearBits(StateManager::getInstance().getHandle(), EVT_LIMIT_SWITCH);
      Logger.verbose(STATE_LOG, "systemEvents after clear bits: %d", xEventGroupGetBits(StateManager::getInstance().getHandle()));
    }
    else if (bits & EVT_CANCEL_BTN)
    {
      Logger.debug(STATE_LOG, "Cancel Button detected");
      Logger.verbose(STATE_LOG, "systemEvents before clear bits: %d", bits);
      currentState = MANUAL_MODE;
      xEventGroupClearBits(StateManager::getInstance().getHandle(), EVT_CANCEL_BTN);
      Logger.verbose(STATE_LOG, "systemEvents after clear bits: %d", xEventGroupGetBits(StateManager::getInstance().getHandle()));
    }
    else if (bits & EVT_TEST_BTN)
    {
      Logger.debug(STATE_LOG, "test Button detected");
      Logger.verbose(STATE_LOG, "systemEvents before clear bits: %d", bits);
      currentState = TEST_MODE;
      xEventGroupClearBits(StateManager::getInstance().getHandle(), EVT_TEST_BTN);
      Logger.verbose(STATE_LOG, "systemEvents after clear bits: %d", xEventGroupGetBits(StateManager::getInstance().getHandle()));
    }


    if (currentState != lastState)
    {
      onStateEnter(currentState);
      onStateExit(lastState);
    }
  }
}

void StateMachine::init()
{
  xTaskCreatePinnedToCore(
    Task::taskEntry,
    "stateMachine",
    10000,
    this,
    0,
    &stateMachineTask,
    1
  )
}