#include "EventGroups.h"

EventGroupHandle_t EventGroups::getHandle()
{
  return systemEvents;
}

EventGroups& EventGroups::getInstance(){
  static EventGroups instance;
  return instance;
}

uint32_t EventGroups::getNumberOfStateTransitions() const
{
  return numberOfStateTransitions;
}

const EventGroups::stateTransitionRule& EventGroups::getStateTransitions(uint32_t index) const
{
  return stateTransitions[index];
}
