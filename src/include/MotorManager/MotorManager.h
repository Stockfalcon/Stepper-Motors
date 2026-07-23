#pragma once
#include <Arduino.h>
#include "include/Globals.h"


/// \ingroup MotorControl
enum MotorCommandType
{
  RUN,
  STOP,
  ENABLE_LIMIT_SWITCH_ALERT,
  CHANGE_DIR
};

/// \ingroup MotorControl
struct MotorCommand
{
  MotorCommandType type;
};

/// \ingroup MotorControl
struct MotorStates{
  bool runMotor = true;
  bool potEnabled = true;
  bool motorDirFwd = true; /// < True runs the motor in the forward direction. 
};

/** \ingroup MotorControl
 * This class controls the stepper motor.
 * It manages acceleration, reads a potentiometer and sets up a timer with a callback that triggers steps.
 */
class MotorManager : public Task{
  public:
  /**
   * \details Initiates the timer that controls the stepper motor, as well as acceleration and main tasks.
   */
    void init();
    void main() override;
    /**
     * This function controls the acceleration of the motor.
     * \param pvParameter Standard parameter for compatibility with xTaskCreatePinnedToCore(). 
     */
    static void motorAccelerationControl(void* pvParameters);
    const uint32_t &getPosition() const;
    static void IRAM_ATTR onStepTimer();
    void clearStepCount();

    uint32_t getSteps();
    static void setTargetStepPeriod_us(uint32_t period_us);
    static uint32_t getTargetStepPeriod_us();
    static void setStepPeriod_us(uint32_t period_us);
    static uint32_t getStepPeriod_us();

    void sendToQueue(const MotorCommand &command);

  private:
    
    void receiveCommands();

    void readPotVal();

    uint32_t position_cm = 0;
    static volatile uint32_t stepPeriod_us;
    static volatile uint32_t targetStepPeriod_us;
    static portMUX_TYPE timerMux; 
    static portMUX_TYPE stepMux; 
    static hw_timer_t *stepTimer;
    static uint32_t stepCount;  // could use inline keyword
    volatile uint32_t potSampleCounter = 0;
    volatile uint32_t accumulatedPotVal = 0;

    MotorStates motorStates;
    TaskHandle_t motorControllerTask = NULL;
    TaskHandle_t motorAccelerationTask = NULL;
    
};