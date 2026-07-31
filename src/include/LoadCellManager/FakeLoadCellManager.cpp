#include "include/LoadCellManager/FakeLoadCellManager.h"
#include <HX711.h>
#include "include/Communication Structures/Queues.h"
#include "include/Logging.h"

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
  for (;;)
  {
    if (loadCellStates.readData)
    {
      while (loadCellStates.readData)
      {
        writeToSerial(readLoadCell());
      }
    }
    vTaskDelay(pdMS_TO_TICKS(20));
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
  printf("strain: %d", data.strain);
}
