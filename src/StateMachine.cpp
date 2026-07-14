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
    Logger.debug(STATE_LOG, "Entered Manual Mode");
    MotorCommand motorCommand{.type = RUN};
    motorController.sendToQueue((MotorCommand*)&motorCommand);
    break;
  }

  case CALIBRATION_MODE:
  {
    Logger.debug(STATE_LOG, "Entered Calibration Mode");
    break;
  }

  case TEST_MODE:
  {
    Logger.debug(STATE_LOG, "Entered Test Mode");
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
    Logger.debug(STATE_LOG, "Exited Manual Mode");
    MotorCommand motorCommand{.type = STOP};
    motorController.sendToQueue((MotorCommand *)&motorCommand);
    break;
  }
  
  case CALIBRATION_MODE:
  {
    Logger.debug(STATE_LOG, "Exited Calibration Mode");
    break;
  }
  
  case TEST_MODE:
  {
    Logger.debug(STATE_LOG, "Exited Test Mode");
    break;
  }
  }
}


void StateMachine::main()
{
  for (;;)
  {
    lastState = currentState;
    // printf("EB handle %p", eventManager.getHandle());
    EventBits_t bits = xEventGroupClearBits(eventManager.getHandle(),0); // maybe b/c getinstance is in IRAM?
    for (int32_t i = 0; i <= eventManager.getNumberOfStateTransitions(); i++)
    {
      auto stateTransition = eventManager.getStateTransitions(i);
      if (bits & stateTransition.trigger)
      {
        Logger.verbose(STATE_LOG, "systemEvents before clear bits: %d", bits);
        xEventGroupClearBits(eventManager.getHandle(), stateTransition.trigger);
        Logger.warning(STATE_LOG, "systemEvents after clear bits: %d", xEventGroupGetBits(eventManager.getHandle()));
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