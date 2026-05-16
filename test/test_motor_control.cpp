#include <Arduino.h>
#include <unity.h>
#include "MotorControl.h"

void test_motor_init(){
  MotorController motor;
  TEST_ASSERT_EQUAL(0, motor.getPosition());
}


void setup(){
  delay(2000);
  UNITY_BEGIN();
  RUN_TEST(test_motor_init);
  UNITY_END();
}

void loop(){
  
}