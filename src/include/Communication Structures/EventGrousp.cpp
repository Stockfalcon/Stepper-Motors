#include "EventGroups.h"

EventGroupHandle_t StateManager::getHandle()
{
  return systemEvents;
}

StateManager &StateManager::getInstance()
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