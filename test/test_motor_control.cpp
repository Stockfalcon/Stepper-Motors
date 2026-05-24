#include <Arduino.h>
#include <unity.h>
#include "MotorControl.h"

void test_motor_initial_position_is_zero() {
  MotorController motor;
  TEST_ASSERT_EQUAL(0, motor.getPosition());
}


