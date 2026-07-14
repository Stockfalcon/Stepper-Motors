#include "LoadCellManager.h"

void LoadCellManager::init()
{
  LoadCellDataQueue = xQueueCreate(10, sizeof(LoadCellData));

  xTaskCreatePinnedToCore(
    Task::taskEntry,
    "LoadCellManager",
    10000,
    this,
    0,
    &loadCellTask,
    1
  );
}

void LoadCellManager::main()
{
  for(;;){
    if(loadCellStates.readData){
      while (loadCellStates.readData){
        sendDataToQueue(readLoadCell());
      }
    }
  }
}

LoadCellData LoadCellManager::readLoadCell()
{

  LoadCellData loadCellData{
    // .stress = hx711.read()
    .time = esp_timer_get_time(),
    .strain = stepsToStrain(motorManager.getSteps())
  };
  return loadCellData;
}

void LoadCellManager::sendDataToQueue(LoadCellData data)
{
  xQueueSendToBack(LoadCellDataQueue, (LoadCellData*)&data, portMAX_DELAY);
}

