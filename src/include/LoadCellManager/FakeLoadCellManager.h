#pragma once
#include <Arduino.h>
#include "include/Globals.h"
#include "include/MotorManager/MotorManager.h"
#include <HX711.h>
#include "include/LoadCellManager/ILoadCellManager.h"

/**
 * \ingroup LoadCell
 * This class was used when I didn't have access to an actual HX711.
 * Instead, I used this class to generate random values for the load cell while keeping core fucntionality.
 */
class FakeLoadCellManager : public ILoadCellManager
{
public:
  /// The class constructor.
  FakeLoadCellManager(MotorManager &motor) : motorManager(motor){}

   /// @brief Initiates the main task.
  void init() override;

  void receiveCommands();

  void main() override;
  /// @brief Sends a data packet to the DataManager.
  /// @param data The data to send. Includes stress, strain and time.
  void sendDataToQueue(LoadCellData data);

  /// @brief Makes up random data to simulate reading a load cell.
  /// @return Data to be transmitted to the DataManager.
  /// @details In addition to reading from the HX711, it also reads the number of steps from the MotorManager and converts this to strain. The stress (force), strain (displacement), and time are put together as a single LoadCellData structure.
  LoadCellData readLoadCell() override;

  /// @brief This stores all calibration data to NVS (Non Volitile Storage) for later use.
  /// @details The data will be kept even if the ESP32 turns off. This will likely be moved to another module some time in the future.
  void writeCalibrationToEEPROM();
  /**
   * Calculates strain based on known milimeters per step (stored in Globals).
   * \param steps This parameter is meant to come from MotorManager::getSteps().
   */
  uint32_t stepsToStrain(uint32_t steps) override;

  void writeToSerial(LoadCellData data);

  private:
    LoadCellStates loadCellStates; // /< Internal states that determine behaviour of the load cell manager.
    MotorManager &motorManager;    // /< A reference to a MotorManager instance.                           
    TaskHandle_t loadCellTask;     // /< The handle used for xTaskCreatePinnedToCore() in init().          

};