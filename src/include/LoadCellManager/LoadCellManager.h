#pragma once
#include <Arduino.h>
#include "include/Globals.h"
#include "include/MotorManager/MotorManager.h"
#include <HX711.h>
#include "include/LoadCellManager/ILoadCellManager.h"


/**
 * \ingroup LoadCell
 * This class reads the force from an HX711 connected to the load cell.
 * It also passes the data to the data manager which further handles it.
  */
class LoadCellManager : public ILoadCellManager{
  public:
    
    /// @brief The class constructor.
    LoadCellManager(MotorManager &motor) : motorManager(motor){}

    /// @brief This initiates the class.
    void init() override;

    void main() override;

    /// @brief This sends a data packet to the DataManager.
    void sendDataToQueue(LoadCellData data) override;

    /// @brief Reads the force applied to the load cell via an HX711 amplifier.
    /// @return Data to be transmitted to the DataManager.
    /// @details In addition to reading from the HX711, it also reads the number of steps from the MotorManager and converts this to strain. The stress (force), strain (displacement), and time are put together as a single LoadCellData structure. 
    LoadCellData readLoadCell() override;

    /// @brief This stores all calibration data to NVS (Non Volitile Storage) for later use. 
    /// @details The data will be kept even if the ESP32 turns off. This will likely be moved to another module some time in the future.
    void writeCalibrationToEEPROM() override;

     /// Calculates strain based on known milimeters per step (stored in Globals).
     ///\param steps This parameter is meant to come from MotorManager::getSteps().
    uint32_t stepsToStrain(uint32_t steps) override;
    private:
      LoadCellStates loadCellStates; ///< Internal states that determine behaviour of the load cell manager.
      MotorManager &motorManager;    ///< A reference to a MotorManager instance.
      TaskHandle_t loadCellTask;     ///< The handle used for xTaskCreatePinnedToCore() in init().
      HX711 hx711;                   ///< The instance that will be used to read and communicate with the HX711.
};