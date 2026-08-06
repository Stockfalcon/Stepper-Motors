#pragma once
#include <Arduino.h>
#include "include/Globals.h"


/// \ingroup MotorControl
/// @brief The types of commands that are available to send to the MotorManager.
enum class MotorCommandType
{
  RUN,
  STOP,
  ENABLE_LIMIT_SWITCH_ALERT,
  CHANGE_DIR
};

/// \ingroup MotorControl
/// @brief This is what is passed through the motorCommandQueue by StateMachine to tell the MotorManager what to do.
/// @details It is set up this way so that in the future, other information can be passed to the MotorManager like numerical values to set speed or position.
struct MotorCommand
{
  MotorCommandType type; ///< The command to send to the MotorManager.
};

/// \ingroup MotorControl
/// @brief These are the internal states of the MotorManager that control what it does.
struct MotorStates{
  bool runMotor = true; ///< Toggles whether the motor is on or off.
  bool potEnabled = true; ///< Toggles whether to read the potentiometer to change the speed of the motor.
  bool motorDirFwd = true; ///< True runs the motor in the forward direction. 
};

/** \ingroup MotorControl
 * This class controls the stepper motor.
 * It manages acceleration, reads a potentiometer and sets up a timer with a callback that triggers steps.
 */
class MotorManager : public Task{
  public:
  /**
   * \brief Initiates the timer that controls the stepper motor, as well as acceleration and main tasks.
   */
    void init();
    void main() override;
    /**
     * This function controls the acceleration of the motor.
     * \param [in] pvParameters Standard parameter for compatibility with xTaskCreatePinnedToCore(). 
     */
    static void motorAccelerationControl(void* pvParameters);

    /// @brief A temporary function used for debugging.
    /// @return Returns position_cm
    const uint32_t &getPosition() const;

    /// @brief A callback function that increases a step counter and moves the motor.
    /// @details Moves the motor by pulling a pin attached to the step pin of a TMC2208 high and low very fast. The speed is controlled by stepPeriod_us.
    static void IRAM_ATTR onStepTimer();

    /// @brief Resets the stepCount to zero.
    void clearStepCount();

    /// @brief Get the current number of steps.
    /// @return The number of steps stored in ::stepCount
    uint32_t getSteps();

    /// @brief uses a mutex to set the targetStepPeriod_us.
    /// @param [in] period_us The amount of time to set in microseconds.
    static void setTargetStepPeriod_us(uint32_t period_us);

    /// @brief Uses a mutex to get the current ::targetStepPeriod_us.
    /// @return Get the current ::targetStepPeriod_us.
    static uint32_t getTargetStepPeriod_us();

    /// @brief uses a mutex to set ::stepPeriod_us.
    /// @param [in] period_us The amount of time to set in microseconds.
    static void setStepPeriod_us(uint32_t period_us);

    /// @brief Uses a mutex to get the current ::stepPeriod_us.
    /// @return Get the current ::stepPeriod_us.
    static uint32_t getStepPeriod_us();

  private:
    /// @brief Reads ::motorCommandQueue and updates any internal states.
    void receiveCommands();

    /// @brief Reads the potentiometer ten times and averages those readings, then updates ::targetStepPeriod_us. 
    void readPotVal();

    uint32_t position_cm = 0; ///< Not used except for testing
    static volatile uint32_t stepPeriod_us;       ///< The current time between steps. This is updated by motorAccelerationControl() to try and match targetStepPeriod_us.
    static volatile uint32_t targetStepPeriod_us; ///< The target time between steps.
    static portMUX_TYPE timerMux; ///< A mutex that prevents the timer ISR from using stepPeriod_us while it is being modified.
    static portMUX_TYPE stepMux; ///< A mutex that prevents other classes from reading steps as they are being modified.

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
    static hw_timer_t *stepTimer; 
     
    static uint32_t stepCount;  ///< A counter that keeps track of the number of steps since its last reset. Does not take skipped steps into account.
    volatile uint32_t potSampleCounter = 0; ///< Counts the number of times the potentiometer has been sampled. Used to get an average reading.
    volatile uint32_t accumulatedPotVal = 0; ///< The sum of all readings from the potentiometer. Used to get an average reading.

    MotorStates motorStates; ///< Internal states used to keep track of what the motor manager should be doing.
    TaskHandle_t motorControllerTask = NULL; ///< Handle for main() FreeRTOS task.
    TaskHandle_t motorAccelerationTask = NULL; ///< Handle for motorAccelerationControl() task.
};