#pragma once
#include <Arduino.h>

extern QueueHandle_t motorCommandQueue;
extern QueueHandle_t loadCellDataQueue;    ///< Sends \ref LoadCellData to the Data Manager
extern QueueHandle_t loadCellCommandQueue; ///< Allows \ref StateMachine to send \ref LoadCellCommand "messages" to the \ref LoadCell Manager.
