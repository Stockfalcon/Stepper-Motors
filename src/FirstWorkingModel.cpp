#include <Arduino.h>

#define DIR_PIN 18
#define EN_PIN 4
#define STEP_PIN 19
// #define STEP_PIN 2
#define POT_PIN 33


volatile int stepPeriod_us = 500;
volatile int potVal;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
hw_timer_t *stepTimer = nullptr;

void IRAM_ATTR onStepTimer(){
  static bool stepState = false;
  stepState = !stepState;
  digitalWrite(STEP_PIN, stepState);
}

void print(void *pvParameters){
  for(;;){
    Serial.print(stepPeriod_us);
    Serial.print(" us");
    Serial.print("  Core");
    Serial.println(xPortGetCoreID());
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void setDelayFromPot(void *pvParameters){
  for(;;){
    potVal = analogRead(POT_PIN);
    portENTER_CRITICAL(&timerMux);
    stepPeriod_us = map (potVal, 0, 4095, 100, 1000);
    Serial.print(stepPeriod_us);
    Serial.print("\n");
    timerAlarmWrite(stepTimer, stepPeriod_us/2, true);
    portEXIT_CRITICAL(&timerMux);
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN,0);
  Serial.begin(115200);



  stepTimer = timerBegin(
    0,
    80,
    true
  );

  timerAttachInterrupt(
    stepTimer,
    &onStepTimer,
    true
  );

  timerAlarmWrite(
    stepTimer,
    stepPeriod_us / 2,
    true
  );

  timerAlarmEnable(stepTimer);
 

  xTaskCreatePinnedToCore(
    setDelayFromPot,
    "delaySetter",
    10000,
    NULL,
    1,
    NULL,
    1
  );
}

void loop() {
  // put your main code here, to run repeatedly:
}

