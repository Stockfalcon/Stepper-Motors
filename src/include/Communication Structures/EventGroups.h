#pragma once
#include <Arduino.h>

//global states
#define EVT_CALIBRATION_BTN (1 << 0)
#define EVT_LIMIT_SWITCH (1 << 1)
#define EVT_CANCEL_BTN (1 << 2)
#define EVT_TEST_BTN (1 << 3)
#define ALERT_SET (1 << 4)
#define INTERLOCK_SET (1 << 5)


  enum systemStates
  {
    MANUAL_MODE,
    CALIBRATION_MODE,
    TEST_MODE,
    ERROR,
    ALERT_MODE
  };
  
  class EventManager
  {
    private:
    // local states(only this class should access)
    
    
    EventGroupHandle_t systemEvents = nullptr;
    static EventGroupHandle_t systemEventsISR;
    
    typedef struct
    { // Transition table for state switcher
      EventBits_t trigger;
      systemStates fromState;
      systemStates toState;
    } stateTransitionRule;
    
    static const u_int32_t numberOfStateTransitions = 5;
    stateTransitionRule stateTransitions[numberOfStateTransitions] = { // Limit switch & Cancel button logic all in main()
      {EVT_LIMIT_SWITCH, ANY_MODE, ALERT_MODE},
      {EVT_CANCEL_BTN, ANY_MODE, ALERT_MODE},
      {EVT_CALIBRATION_BTN, ALERT_MODE, MANUAL_MODE},
      {EVT_CALIBRATION_BTN, MANUAL_MODE, CALIBRATION_MODE},
      {EVT_TEST_BTN, CALIBRATION_MODE, TEST_MODE}}; 
      
    public:
      void init();
      EventGroupHandle_t getHandle();
      static EventGroupHandle_t getHandleFromISR();
      
      static EventManager &getInstance();
      
      uint32_t getNumberOfStateTransitions() const;
      const stateTransitionRule &getStateTransitions(uint32_t index) const;
      systemStates ANY_MODE;
};