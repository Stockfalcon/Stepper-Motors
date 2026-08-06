#pragma once
#include <Arduino.h>

//global events
#define EVT_CALIBRATION_BTN (1 << 0)
#define EVT_LIMIT_SWITCH (1 << 1)
#define EVT_CANCEL_BTN (1 << 2)
#define EVT_TEST_BTN (1 << 3)
#define ALERT_SET (1 << 4)
#define INTERLOCK_SET (1 << 5)

/// @brief  Global states used primarily by StateMachine to keep track of the tensile tester's current state. 
enum systemStates
{
  MANUAL_MODE,
  CALIBRATION_MODE,
  TEST_MODE,
  ERROR,
  ALERT_MODE
};

  /// @brief  This class keeps track of
  class EventManager
  {
    private: 
    /// @brief A handle to the event groups for the tensile tester.
    EventGroupHandle_t systemEvents = nullptr;

    /// @brief A static handle to the event groups for the tensile tester. Used in the ButtonManager ISRs (Interrupt Service Routines).
    static EventGroupHandle_t systemEventsISR;

    /// @brief Transition table structure used in stateTransitions.
    /// @details very important for the functioning of StateMachine::main(). 
    typedef struct
    {
      EventBits_t trigger; ///< The trigger to initiate a transition. It should be an event bit like EVT_CANCEL_BTN.
      systemStates fromState; ///< The state to transition from (compare with StateMachine::currentState).
      systemStates toState; ///< The state to transition to.
    } stateTransitionRule;

    /// @brief The number of defined stateTransitions.
    static const u_int32_t numberOfStateTransitions = 5;

    /// @brief The transition table that defines how to change state based on events and current state.
    /// @details When deciding to do a transition, BOTH the trigger and current state must match one of the defined transitions.
    stateTransitionRule stateTransitions[numberOfStateTransitions] = { // Limit switch & Cancel button logic all in main()
      {EVT_LIMIT_SWITCH, ANY_MODE, ALERT_MODE},
      {EVT_CANCEL_BTN, ANY_MODE, ALERT_MODE},
      {EVT_CALIBRATION_BTN, ALERT_MODE, MANUAL_MODE},
      {EVT_CALIBRATION_BTN, MANUAL_MODE, CALIBRATION_MODE},
      {EVT_TEST_BTN, CALIBRATION_MODE, TEST_MODE}}; 
      
    public:
      /// @brief Creates the event group and sets systemEventsISR.
      void init();
      /// @brief Get the tensile tester's event group handle.
      /// @return The systemEvents handle.
      EventGroupHandle_t getHandle();
      
      /// @brief Get the tensile tester's event group handle from an ISR (Interrupt Service Routine).
      /// @return The static systemEventsISR handle (same as systemEvents but static).
      static EventGroupHandle_t getHandleFromISR();
      
      /// @brief Used to get an instance to this class.
      /// @returns This class' instance.
      /// @deprecated This class used to be a singleton.
      static EventManager &getInstance();
      
      /// @brief Used by the StateMachine to loop through all the state transitions in StateMachine::main().
      /// @returns The ::numberOfStateTransitions.
      uint32_t getNumberOfStateTransitions() const;

      /// @brief Get the ::stateTransitions. Used primarily by StateMachine::main().
      /// @param index Which state transition to return.
      /// @return A reference to one of the ::stateTransitions.
      const stateTransitionRule &getStateTransitions(uint32_t index) const;

      /// @brief Used to transition from any mode. Typically used when a cancel button or limit switch is hit and everything must stop.
      systemStates ANY_MODE;
};