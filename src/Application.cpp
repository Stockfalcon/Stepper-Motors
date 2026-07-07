#include "Application.h"
#include "include/PinMap.h"
#include "include/Logging.h"

void Application::init(){
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  Logger.debug(APP_LOG, "Initializations started");
  stateManager.init();
  buttonManager.init();
  motorManager.init();
  stateMachine.init();
  Logger.debug(APP_LOG, "Initializations complete");
}