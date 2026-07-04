#include "Application.h"

void Application::init(){
  buttonManager.init();
  motorManager.init();
  stateMachine.init();
}