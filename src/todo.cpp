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