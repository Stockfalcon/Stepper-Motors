#include <Arduino.h>
#include <unity.h>
#include "EventGroups.h"

void test_get_number_of_state_transitions() {
  TEST_ASSERT_EQUAL(2, EventGroups::getInstance().getNumberOfStateTransitions());
}

void test_state_transition_rules() {
  const auto &rule0 = EventGroups::getInstance().getStateTransitions(0);
  TEST_ASSERT_EQUAL(EVT_CALIBRATION_BTN, rule0.trigger);
  TEST_ASSERT_EQUAL(MANUAL_MODE, rule0.fromState);
  TEST_ASSERT_EQUAL(CALIBRATION_MODE, rule0.toState);

  const auto &rule1 = EventGroups::getInstance().getStateTransitions(1);
  TEST_ASSERT_EQUAL(EVT_TEST_BTN, rule1.trigger);
  TEST_ASSERT_EQUAL(MANUAL_MODE, rule1.fromState);
  TEST_ASSERT_EQUAL(TEST_MODE, rule1.toState);
}

