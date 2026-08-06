#pragma once
#include <Arduino.h>
#include "include/PinMap.h"
#include "include/Logging.h"

/// @brief This class is for testing the stepper motor independently of any other modules.
class StepperTest
{
private:
  /**
   * @brief A timer with an alarm attached that triggers onStepTimer() to move the motor.
   * @details This is initialized as follows:
   * ~~~~~{.cpp}
    stepTimer = timerBegin(
      0,   // timer number (the options are 0,1,2,3) for my ESP32 model
      80,  // prescale divider (clock is 80 MHz)
      true // count up (true) or down (false)
    );

    timerAttachInterrupt( // when timer is triggered call onStepTimer()
      stepTimer,
      &onStepTimer,
      true // edge triggered timing
    );

    timerAlarmWrite( // write amount of time to trigger alarm to the timer
      stepTimer,
      stepPeriod_us / 2, // divide by two for high AND low
      true);

    timerAlarmEnable(stepTimer); // start the timer
   * ~~~~~
   */
  hw_timer_t *stepTimer = nullptr;        
  volatile uint32_t stepPeriod_us = 500;   ///< The current time between steps. This is updated by motorAccelerationControl() to try and match targetStepPeriod_us.
  volatile uint32_t accumulatedPotVal = 0; ///< The sum of all readings from the potentiometer. Used to get an average reading.
  volatile uint32_t potSampleCounter = 0;  ///< Counts the number of times the potentiometer has been sampled. Used to get an average reading.
  static portMUX_TYPE timerMux;            ///< A mutex that prevents the timer ISR from using stepPeriod_us while it is being modified.
  TaskHandle_t motorControllerTask = NULL; ///< Handle for main() FreeRTOS task.
  static uint32_t stepCount;               ///< A counter that keeps track of the number of steps since its last reset. Does not take skipped steps into account.

public:
  /// @brief A callback function that increases a step counter and moves the motor.
  /// @details Moves the motor by pulling a pin attached to the step pin of a TMC2208 high and low very fast. The speed is controlled by stepPeriod_us.
  static void IRAM_ATTR onStepTimer();

  /// @brief Used to statically cast readPotVal().
  /// @param [in] pvParameters Required for xTaskCreate() used in init()
  /// @details It serves the same purpose as Task::taskEntry() without needing to inherit.
  static void castReadPotVal(void *pvParameters);

  /// @brief Reads the potentiometer ten times and averages those readings, then updates ::stepPeriod_us. 
  void readPotVal();

  /// @brief Initiates the timer that controls the stepper motor, as well as acceleration and main tasks.
  void init();

  };