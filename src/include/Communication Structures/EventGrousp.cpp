#include "EventGroups.h"

// IRAM_ATTR because this is accessed by button interrups.
EventGroupHandle_t IRAM_ATTR EventManager::getHandle()
{
  return systemEvents;
}
// IRAM_ATTR because this is accessed by button interrups.
EventManager IRAM_ATTR &EventManager::getInstance()
{
  static EventManager instance;
  return instance;
}

uint32_t EventManager::getNumberOfStateTransitions() const
{
  return numberOfStateTransitions;
}

const EventManager::stateTransitionRule &EventManager::getStateTransitions(uint32_t index) const
{
  return stateTransitions[index];
}

void EventManager::init()
{
  systemEvents = xEventGroupCreate();
}