#pragma once
#include <Arduino.h>

extern QueueHandle_t motorCommandQueue;    ///< Allows StateMachine to send MotorCommand messages to the MotorManager.
extern QueueHandle_t loadCellDataQueue;    ///< Sends \ref LoadCellData to the Data Manager.
extern QueueHandle_t loadCellCommandQueue; ///< Allows StateMachine to send LoadCellCommand messages to the LoadCellManager.
