#include <Arduino.h>

enum MotorCommandType{
  RUN,
  STOP,
  ENABLE_LIMIT_SWITCH_ALERT
};

struct MotorCommand
{
  MotorCommandType type;
};

QueueHandle_t MotorCommandQueue = xQueueCreate(10, sizeof(MotorCommand));