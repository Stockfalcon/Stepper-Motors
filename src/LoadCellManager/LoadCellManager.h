#pragma once
#include <Arduino.h>
#include "include/Globals.h"
#include "include/MotorManager/MotorManager.h"

struct LoadCellData
{
  uint32_t stress; //force
  int64_t time;
  uint32_t strain; // displacement
};

struct LoadCellStates
{
  bool readData = false;
  bool writeToEEPROM = false;
};

class LoadCellManager : public Task{
  public:
    LoadCellManager(MotorManager motor) : motorManager(motor){}
    void init();
    void main() override;
    void sendDataToQueue(LoadCellData data);
    LoadCellData readLoadCell();
    void writeCalibrationToEEPROM();
    uint32_t stepsToStrain(uint32_t steps);

  private : QueueHandle_t LoadCellDataQueue;
    LoadCellStates loadCellStates;
    MotorManager motorManager;
    TaskHandle_t loadCellTask;


};