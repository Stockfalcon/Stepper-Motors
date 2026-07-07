#include "EventGroups.h"

EventGroupHandle_t IRAM_ATTR StateManager::getHandle()
{
  return systemEvents;
}

StateManager IRAM_ATTR &StateManager::getInstance()
{
  static StateManager instance;
  return instance;
}

uint32_t StateManager::getNumberOfStateTransitions() const
{
  return numberOfStateTransitions;
}

const StateManager::stateTransitionRule &StateManager::getStateTransitions(uint32_t index) const
{
  return stateTransitions[index];
}

void StateManager::init()
{
  systemEvents = xEventGroupCreate();
}