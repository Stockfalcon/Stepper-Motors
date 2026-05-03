#include <Arduino.h>

#define DIR_PIN 18
#define EN_PIN 0
#define STEP_PIN 19
#define POT_PIN 25
#define pass (void)0
#define stepAccel 10 // steps / us^2

#define BIT_0 (1 << 0)//* ManualMode
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
#define BIT_3 (1 << 4)

hw_timer_t *stepTimer = nullptr;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

EventGroupHandle_t systemEventStates = xEventGroupCreate();


enum systemStates{
  MANUAL_MODE,
  CALIBRATION_MODE,
  TEST_MODE,
  ABORT
};
enum systemStates currentState = MANUAL_MODE; // always start in manual mode

TaskHandle_t manualMoveTask = NULL;
TaskHandle_t systemStateSwitcherTask = NULL;

volatile uint32_t targetStepPeriod_us = 200; 
volatile uint32_t stepPeriod_us = 200;       // initial speed

void IRAM_ATTR onStepTimer(){ // linked to hardware timer interupt
    static bool stepState = false;
    stepState = !stepState;
    digitalWrite(STEP_PIN, stepState);
}

void setStepperSpeed(uint32_t period_us){          // call from any core or task (should only really be called by motorControl)
  portENTER_CRITICAL(&timerMux);                   // temporairily disable interrupt and prevent ISR from running mid update
  stepPeriod_us = period_us;                      
  timerAlarmWrite(stepTimer, period_us / 2, true); // update timer alarm period                                             
  portEXIT_CRITICAL(&timerMux);
}

void motorControl(){                               // pinned to core 0 (core 0's only task)
  for(;;){                                        
    if (targetStepPeriod_us < stepPeriod_us){      // decrease speed  
      setStepperSpeed(stepPeriod_us -= stepAccel);
      if (targetStepPeriod_us > stepPeriod_us){    // detect overshoot
          setStepperSpeed(targetStepPeriod_us);   
      }
    }
    else if (targetStepPeriod_us > stepPeriod_us){ // increase speed  
      setStepperSpeed(stepPeriod_us += stepAccel);
      if (targetStepPeriod_us > stepPeriod_us){    // detect overshoot
          setStepperSpeed(targetStepPeriod_us);
      } 
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void manualMode(void *pvParameters){
  uint32_t accumulatedPotVal = 0;
  int counter = 0;
  for(;;){
    counter++;
    accumulatedPotVal += analogRead(POT_PIN);
    if(counter == 10){
      uint32_t avgPotVal = accumulatedPotVal / 10;
      accumulatedPotVal = 0;
      counter = 0 ;
      uint32_t period_us = map(avgPotVal, 0, 4095, 1000, 200);
      targetStepPeriod_us = period_us;
      Serial.print(period_us);
      Serial.print("  Core");
      Serial.println(xPortGetCoreID());
    }
    
    if (currentState != MANUAL_MODE){
      vTaskSuspend(manualMoveTask); // ? will this force freeRTOS to exit this function and tansition to systemTask?
    }
    vTaskDelay(pdMS_TO_TICKS(1)); // run at ~1kHz
  }
}

void calibrationMode(){ // ! Not implemented yet
  pass;
}                      

void testMode(){        // ! Not implemented yet
  pass;                
}

void systemStateSwitcher(void *pvParameters){
  for(;;){
    switch (currentState){

      case MANUAL_MODE:
        xTaskCreatePinnedToCore(
          manualMode,
          "motor",
          10000,
          NULL,
          1,
          &manualMoveTask,
          1
        );
        break;

      case CALIBRATION_MODE:
        calibrationMode();
        break;

      case TEST_MODE:
        testMode();
        break;
    }
  }
}

void setup() {
  // *put your setup code here, to run once:
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  Serial.begin(115200);

  stepTimer = timerBegin(
    0,      //timer number (the options are 0,1,2,3)
    80,     //prescale divider (clock is 80 MHz)
    true    //count up (true) or down (false)
  );  
    
  timerAttachInterrupt( // when timer is triggered call onStepTimer()
    stepTimer,         
    &onStepTimer,      
    true                // edge triggered timing                     
  );

  timerAlarmWrite(     // write amount of time to trigger alarm to the timer
    stepTimer,        
    stepPeriod_us / 2, // divide by two for high AND low                    
    true
  );

  timerAlarmEnable(stepTimer);    //start the timer

  xTaskCreatePinnedToCore(
    systemStateSwitcher,
    "systemStateSwitcher",
    10000,
    NULL,
    1,
    &systemStateSwitcherTask,
    1
  );
}

void loop() {
  // *put your main code here, to run repeatedly:
}



// * All additions to make to the tensile tester program

// xTODO: Add enums and system state tasks
// xTODO: Edit existing funcxtions to match enum format

// xTODO: Add acceleration/deceleration function
// TODO: Add change direction function (GPIO interupt)
// TODO: Add start/stop functions (GPIO Interupt)
// TODO: Connect start/stop and direction to two pins (to be controlled by DPDT switch)

// TODO: Add limit switch functionality to switch direction or stop

// TODO: Add load cell reading capabilities (HX711)
// ? best option would probably have this be read by ISR trggered by hardware clock
// ? what to do with the saved information after this? a queue?

// TODO:  Add load cell calibration function
// TODO:  Add 4x4 number pad functionality to enter load cell calibration numbers
// ? possible to include LCD display solely used for calibration?
// ? chat gpt recomends storing calibration in NVS flash (what is this? why not use EEPROM? same thing?)

// TODO: add steper motor step count functionality to calculate strain (distance calculation)
// TODO: Add loadQueue for inter task communication

// TODO: Add stepper motor calibration (using limit switches on GPIO interupts)
// ? should each test start with a calibration

// TODO: Create basic test mode function (No data logging yet)
// TODO: Add emergency stop button! (GPIO interupt)

// TODO: At the start of a program, should be able to move the stepper motor (using DPDT switch), start calibration (using a button), or enter test mode (using another button)

// TODO: Add SD card logging capabilities 
// ? should sd card logging be on a seperate core? (would this even be possible given the 1 us delay in the other core?)
// ? maybe have it so that you cannot change the speed after a program has started

// TODO: Add (bluetooth?) serial functionality to also send data to computer/phone for real time visualisation
// ? would arduino science journal app be suitable for this?

// * Chat GPT recomendatiions
// define system states using enum (IDLE, MANUAL_MODE, HOMING, CALIBRATION, TEST_RUNNING, ERROR)
// fault handling (Limit switch, load cell overload, sd card write faliure, motor stall timeout) by logging (separate file?)
// SD logging data integrity ( buffer writes, periodic flush, headers with calibration info)
// logging inclusions (always include calibration constants, step/mm, sample rate, test start timestamp)

/* 
Core 0{
  ISR stepTask (+ step counter)
  Motion control (accel, decel, start, stop)
}
Core 1{
  readPot / setStepSpeed (only during manual mode)
  read HX711 (test mode/thread 1)
  log data (testmode/thread 2)
  buttonPressHandler (part of all modes)
  keypad input (part of callibration mode)
  LCD output (part of calibration mode)

}
Communication{
  loadQueue (for sending load cell data between threads)
}

*/


// TODO: look into state machine diagrams and/or state task diagrams