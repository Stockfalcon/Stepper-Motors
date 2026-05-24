#include <Arduino.h>
#include <unity.h>
#include "StateMachine.h"

void test_state_machine_enter_exit_no_crash() {
  StateMachine stateMachine;
  stateMachine.onStateEnter(MANUAL_MODE);
  stateMachine.onStateEnter(CALIBRATION_MODE);
  stateMachine.onStateEnter(TEST_MODE);

  stateMachine.onStateExit(MANUAL_MODE);
  stateMachine.onStateExit(CALIBRATION_MODE);
  stateMachine.onStateExit(TEST_MODE);

  TEST_ASSERT_TRUE(true);
}

