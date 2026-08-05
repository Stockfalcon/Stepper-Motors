#pragma once
#include <Arduino.h>
#include "include/Globals.h"
#include "include/MotorManager/MotorManager.h"
#include <HX711.h>


/// \ingroup LoadCell
/// Structure used to send data to data manager via loadCellDataQueue (Queues.h).

struct LoadCellData
{
  int stress; ///< Force.
  int64_t time; ///< Time of measurement. 
  uint32_t strain; ///< Displacement. Calculated from counted number of steps in MotorManager.
};

/// \ingroup LoadCell
/// Commands sent by StateManager used to set internal LoadCellManager::LoadCellStates.
enum class LoadCellCommandType
{
  GET_DATA,
  STOP
};

///\ingroup LoadCell
/// Structure used to send data to data manager via loadCellCommandQueue (Queues.h).
struct LoadCellCommand
{
  LoadCellCommandType type;
};

/// \ingroup LoadCell
/// Internal states that determine behaviour of the load cell manager.
/// These states are determined by commands sent throght the load cell's command queue by the state manager.

struct LoadCellStates
{
  bool readData = true;
  bool writeToEEPROM = false;
}; 





/// \ingroup LoadCell
/// @brief This class reads the force from an HX711 connected to the load cell.
/// @details It also passes the data to the data manager which further handles it.
class ILoadCellManager : public Task
{
public:
  virtual void init() = 0; ///< Include a function that initializes all taks and required setups.
  virtual void main()= 0; ///< Include a main() task that will do all the work. 
  virtual LoadCellData readLoadCell() = 0; ///< Include a function to read from the HX711 load cell amplifier.
  virtual uint32_t stepsToStrain(uint32_t steps) = 0; ///< Include a function that will take the number of steps from MotorManager::stepCount and convert it to strain (displacement).

};