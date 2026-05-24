#include <Arduino.h>
#include <unity.h>
#include "EventGroups.h"

void test_get_number_of_state_transitions(){
  TEST_ASSERT_EQUAL(2, EventGroups::getInstance().getNumberOfStateTransitions());
}

void setup(){
  delay(2000);
  UNITY_BEGIN();
  RUN_TEST(test_get_number_of_state_transitions);
  UNITY_END();
}