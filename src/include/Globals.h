#pragma once
// defining all these values in their respective .h files apparently creates a new symbol
//  for every .cpp file that includes it. This lead to duplicates of each symbol at link time
//  and the compiler didn't like that.



// TaskHandle_t manualModeTask = NULL;
// TaskHandle_t calibrationModeTask = NULL;
// TaskHandle_t testModeTask = NULL;
// TaskHandle_t systemStateSwitcherTask = NULL;

/// @brief All device managers inherit from this class.
/// @details This class provides each inherited class with a main() function that can be used with xTaskCreate() and become a FreeRTOS task. In order for a function to be a task, it must be static, as well as any other variables and functions it reads, writes, or calls. If another class inherits from Task, main() doesn't have to be static; instead, you can call taskEntry() to run main as a static function.
class Task{
  public:
  /// @brief This contains all of the code that will run during a 
  /// @details In order for this to run properly as a task, the definition MUST contain a forever loop and vTaskDelay().
  virtual void main() = 0;

  /**
  @brief This will call a static version of main().
  @param pvParameters Required for xTaskCreate().
  @details This function should be used in a class' init() as follows.
  * ~~~~~{.cpp}
  void MyClass::init(){
    xTaskCreate(
      Task::taskEntry,
      "myClass",   // Task Name
      10000,       // usStackDepth
      this,        // pvParameters
      1,           // Priority
      &myClassTask // Reference to a TaskHandle_t
      );
  }
  * ~~~~~
  */
  static void taskEntry(void *pvParameters)
  {
    printf("pvParameters = %p\n", pvParameters);
    Task *self = static_cast<Task*>(pvParameters);
    self->main();
    printf("self = %p\n", self);
  }
};