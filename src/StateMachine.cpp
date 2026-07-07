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
    xEventGroupSetBits(EventManager::getInstance().getHandle(), STATE_MANUAL_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_MANUAL_ACTIVE set");
    MotorCommand motorCommand{
        .type = RUN};
    motorController.sendToQueue((MotorCommand*)&motorCommand);
    break;
  }

  case CALIBRATION_MODE:
  {
    xEventGroupSetBits(EventManager::getInstance().getHandle(), STATE_CALIBRATION_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_CALIBRATION_ACTIVE set");
    break;
  }

  case TEST_MODE:
  {
    xEventGroupSetBits(EventManager::getInstance().getHandle(), STATE_CALIBRATION_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_TEST_ACTIVE set");
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
    xEventGroupClearBits(EventManager::getInstance().getHandle(), STATE_MANUAL_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_MANUAL_ACTIVE reset");
    MotorCommand motorCommand{
      .type = STOP
    };
    motorController.sendToQueue((MotorCommand *)&motorCommand);
    break;
  }

  case CALIBRATION_MODE:
  {
    xEventGroupClearBits(EventManager::getInstance().getHandle(), STATE_CALIBRATION_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_CALIBRATION_ACTIVE reset");
    break;
  }

  case TEST_MODE:
  {
    xEventGroupClearBits(EventManager::getInstance().getHandle(), STATE_TEST_ACTIVE);
    Logger.debug(STATE_LOG, "STATE_TEST_ACTIVE reset");
    break;
  }
  }
}


void StateMachine::main()
{
  for (;;)
  {
    lastState = currentState;
    EventBits_t bits = xEventGroupGetBits(EventManager::getInstance().getHandle());
    for (int32_t i = 0; i <= EventManager::getInstance().getNumberOfStateTransitions(); i++)
    {
      auto stateTransition = EventManager::getInstance().getStateTransitions(i);
      if (bits & stateTransition.trigger)
      {
        Logger.verbose(STATE_LOG, "systemEvents before clear bits: %d", bits);
        xEventGroupClearBits(EventManager::getInstance().getHandle(), stateTransition.trigger);
        Logger.verbose(STATE_LOG, "systemEvents after clear bits: %d", xEventGroupGetBits(EventManager::getInstance().getHandle()));
        if (currentState == stateTransition.fromState){
          currentState = stateTransition.toState;
        }
      }
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
  );
}