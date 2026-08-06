#include "include/LoadCellManager/FakeLoadCellManager.h"
#include <HX711.h>
#include "include/Communication Structures/Queues.h"
#include "include/Logging.h"
#include "include/PinMap.h"

void FakeLoadCellManager::init()
{
  xTaskCreatePinnedToCore(
      Task::taskEntry,
      "LoadCellManager",
      10000,
      this,
      2,
      &loadCellTask,
      1);

      Logger.debug(LOAD_CELL_LOG, "Initializations finnished");
}

void FakeLoadCellManager::main()
{
  const TickType_t xPeriod = pdMS_TO_TICKS(20);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;)
  {
    receiveCommands();
    if (loadCellStates.readData)
    {
      writeToSerial(readLoadCell());
    }
    vTaskDelayUntil(&xLastWakeTime, xPeriod);
  }
}


void FakeLoadCellManager::receiveCommands(){
  LoadCellCommand message{};
  if (xQueueReceive(motorCommandQueue, &message, 0) == pdTRUE)
  {
    switch (message.type)
    {
    case (LoadCellCommandType::GET_DATA):
      loadCellStates.readData = true;
      break;

    case (LoadCellCommandType::STOP):
      loadCellStates.readData = false;
      loadCellStates.writeToEEPROM = false;
      break;
    }
  }
  else
  {
    // Logger.debug(LOAD_CELL_LOG, "Load Cell Controller failed to receive command from queue.");
  }
}

LoadCellData FakeLoadCellManager::readLoadCell()
{
  uint64_t timeNow = esp_timer_get_time();
  srand(timeNow);
  LoadCellData loadCellData{
    .stress = rand() % 101,
    .time = timeNow,
    .strain = stepsToStrain(motorManager.getSteps())
  };
  return loadCellData;
}

void FakeLoadCellManager::sendDataToQueue(LoadCellData data)
{
  xQueueSendToBack(loadCellDataQueue, (LoadCellData *)&data, portMAX_DELAY);
}

void FakeLoadCellManager::writeCalibrationToEEPROM(){};


uint32_t FakeLoadCellManager::stepsToStrain(uint32_t steps){
  return steps *2;
}

void FakeLoadCellManager::writeToSerial(LoadCellData data){
  printf("time: %d\nstrain: %d\nstress: %d\n\n",data.time, data.strain, data.stress);
}
