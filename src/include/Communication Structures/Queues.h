#pragma once
#include <Arduino.h>

extern QueueHandle_t motorCommandQueue = nullptr;
extern QueueHandle_t loadCellDataQueue = nullptr;    ///< Sends \ref LoadCellData to the Data Manager
extern QueueHandle_t loadCellCommandQueue = nullptr; ///< Allows \ref StateMachine to send \ref LoadCellCommand "messages" to the \ref LoadCell Manager.
