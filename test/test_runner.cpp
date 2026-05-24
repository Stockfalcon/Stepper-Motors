#include <Arduino.h>
#include <unity.h>

void test_motor_initial_position_is_zero();
void test_get_number_of_state_transitions();
void test_state_transition_rules();
void test_state_machine_enter_exit_no_crash();

void setUp() {}
void tearDown() {}

void setup() {
  delay(2000);
  UNITY_BEGIN();
  RUN_TEST(test_motor_initial_position_is_zero);
  RUN_TEST(test_get_number_of_state_transitions);
  RUN_TEST(test_state_transition_rules);
  RUN_TEST(test_state_machine_enter_exit_no_crash);
  UNITY_END();
}

void loop() {}
