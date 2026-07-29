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
    
    // The class constructor.
    // LoadCellManager(MotorManager &motor) : motorManager(motor){}
    /**
     * Initiates the main task.
     */
    void init() override;
    void main() override;
    /// Sends a data packet to the DataManager.
    void sendDataToQueue(LoadCellData data) override;
    LoadCellData readLoadCell() override;
    void writeCalibrationToEEPROM() override;
    /**
     * Calculates strain based on known milimeters per step (stored in Globals).
     * \param steps This parameter is meant to come from MotorManager::getSteps().
     */
    uint32_t stepsToStrain(uint32_t steps) override;
};