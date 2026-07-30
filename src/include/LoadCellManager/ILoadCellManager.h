#pragma once
#include <Arduino.h>
#include "include/Globals.h"
#include "include/MotorManager/MotorManager.h"
#include <HX711.h>

/**
 * \ingroup LoadCell
 * Structure used to send data to data manager via \ref loadCellDataQueue.
 */
struct LoadCellData
{
  uint32_t stress; ///< Force
  int64_t time;
  uint32_t strain; ///< Displacement
};

/**
 * \ingroup LoadCell
 * Commands sent by StateManager used to set internal \ref LoadCellStates.
 */
enum LoadCellCommandType
{
  GET_DATA,
};
/**
 * \ingroup LoadCell
 * Structure used to send data to data manager via \ref loadCellCommandQueue.
 */
struct LoadCellCommand
{
  LoadCellCommandType type;
};

/**
 * \ingroup LoadCell
 * Internal states that determine behaviour of the load cell manager.
 * These states are determined by commands sent throght the load cell's command queue by the state manager.
 */
struct LoadCellStates
{
  bool readData = false;
  bool writeToEEPROM = false;
};





/**
 * \ingroup LoadCell
 * This class reads the force from an HX711 connected to the load cell.
 * It also passes the data to the data manager which further handles it.
 */
class ILoadCellManager : public Task
{
public:
  /// The class constructor.
  ILoadCellManager(MotorManager &motor) : motorManager(motor) {}
  /**
   * Initiates the main task.
   */
  virtual void init() = 0;
  //! virtual void main()= 0; 
  /// Sends a data packet to the DataManager.
  virtual void sendDataToQueue(LoadCellData data) = 0;
  virtual LoadCellData readLoadCell() = 0;
  virtual void writeCalibrationToEEPROM() = 0;
  /**
   * Calculates strain based on known milimeters per step (stored in Globals).
   * \param steps This parameter is meant to come from MotorManager::getSteps().
   */
  virtual uint32_t stepsToStrain(uint32_t steps) = 0;

protected:
  LoadCellStates loadCellStates; ///< Internal states that determine behaviour of the load cell manager.
  MotorManager &motorManager;    ///< A reference to a MotorManager instance.
  TaskHandle_t loadCellTask;     ///< The handle used for xTaskCreatePinnedToCore() in init().
  HX711 hx711;
};