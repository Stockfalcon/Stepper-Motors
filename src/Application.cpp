#include "Application.h"
#include "include/PinMap.h"
#include "include/Logging.h"
#include "include/Communication Structures/Queues.h"

void Application::init(){
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  motorCommandQueue = xQueueCreate(10, sizeof(MotorCommand));
  if (motorCommandQueue == nullptr)
  {
    Logger.error(APP_LOG, "Motor command queue creation failed");
    return;
  }
  loadCellCommandQueue = xQueueCreate(10, sizeof(MotorCommand));
  if (loadCellCommandQueue == nullptr)
  {
    Logger.error(APP_LOG, "Load cell command queue creation failed");
    return;
  }
  loadCellDataQueue = xQueueCreate(10, sizeof(MotorCommand));
  if (loadCellDataQueue == nullptr)
  {
    Logger.error(APP_LOG, "Load cell data queue creation failed");
    return;
  }



  Logger.debug(APP_LOG, "Initializations started");
  eventManager.init();
  buttonManager.init();
  motorManager.init();
  stateMachine.init();
  Logger.debug(APP_LOG, "Initializations complete");
}