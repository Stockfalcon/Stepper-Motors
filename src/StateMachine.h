#pragma once
#include "include/Communication Structures/EventGroups.h"
#include "include/MotorManager/MotorManager.h"
/// @brief This class controls how the tensile tester reacts to events (button presses).
class StateMachine : public Task
{
public:
  /// @brief Constructor
  /// @param controller A reference to \ref MotorManager instance
  /// @param events A reference to \ref EventManager instance
  StateMachine(MotorManager &controller, EventManager& events) : motorController(controller), eventManager(events) {}
  
  /// @brief Uses Event Group bits set by \ref ButtonManager to determine system state.
  /// The state transitions are determined by the state transition table in \reg EventGroups. It calls onStateEnter() and onStateExit() when transitioning between states.
  void main() override;
  
  /// @brief Passes commands to the various modules via their respective queues.
  /// @param state The state being enetered. This is determined by the transition table in \ref EventGroups
  void onStateEnter(systemStates state);
  
  /// @brief Passes commands to the various modules via their respective queues.
  /// @param state The state being exited. This is determined by the transition table in \ref EventGroups
  void onStateExit(systemStates state);
  
  /// @brief initializes the main task.
  void init();

private:
  systemStates currentState = MANUAL_MODE; /// <Always start in manual mode.
  systemStates lastState; ///< Helps determine when state transitions occur.
  MotorManager &motorController;
  EventManager &eventManager;
  TaskHandle_t stateMachineTask;
};