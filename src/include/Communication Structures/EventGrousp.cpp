#include "EventGroups.h"

EventGroupHandle_t EventManager::systemEventsISR = nullptr;

// IRAM_ATTR because this is accessed by button interrups.
EventGroupHandle_t EventManager::getHandle()
{
  return systemEvents;
}

EventGroupHandle_t EventManager::getHandleFromISR()
{
  return systemEventsISR;
}

EventManager &EventManager::getInstance()
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
  systemEventsISR = systemEvents;
}