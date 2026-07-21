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

enum LoadCellCommandType{
  GET_DATA,
};

struct LoadCellCommand
{
  LoadCellCommandType type;
};

/**
 * Internal states that determine behaviour of the load cell manager.
 * These states are determined by commands sent throght the load cell's command queue by the state manager.
  */
struct LoadCellStates
{
  bool readData = false;
  bool writeToEEPROM = false;
};
/**
 * This class reads the force from an HX711 connected to the load cell.
 * It also passes the data to the data manager which further handles it.
  */
class LoadCellManager : public Task{
  public:
    
    /// The class constructor.
    LoadCellManager(MotorManager &motor) : motorManager(motor){}
    /**
     * Initiates the main task.
     */
    void init();
    void main() override;
    /// Sends a data packet to the DataManager.
    void sendDataToQueue(LoadCellData data);
    LoadCellData readLoadCell();
    void writeCalibrationToEEPROM();
    /// Calculates strain based on known milimeters per step (stored in Globals).
    uint32_t stepsToStrain(uint32_t steps);

  private : 
    QueueHandle_t loadCellDataQueue;
    QueueHandle_t loadCellCommandQueue;
    LoadCellStates loadCellStates; ///< Internal states that determine behaviour of the load cell manager.
    MotorManager &motorManager; ///< A reference to a MotorManager instance.
    TaskHandle_t loadCellTask;


};