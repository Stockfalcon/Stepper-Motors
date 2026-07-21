#pragma once
// defining all these values in their respective .h files apparently creates a new symbol
//  for every .cpp file that includes it. This lead to duplicates of each symbol at link time
//  and the compiler didn't like that.



// TaskHandle_t manualModeTask = NULL;
// TaskHandle_t calibrationModeTask = NULL;
// TaskHandle_t testModeTask = NULL;
// TaskHandle_t systemStateSwitcherTask = NULL;


class Task{
  public:
  virtual void main() = 0;
  static void taskEntry(void *pvParameters)
  {
    printf("pvParameters = %p\n", pvParameters);
    Task *self = static_cast<Task*>(pvParameters);
    self->main();
    printf("self = %p\n", self);
  }
};