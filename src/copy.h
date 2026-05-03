#include <Arduino.h>

#define DIR_PIN 18
#define EN_PIN 18
#define STEP_PIN 19
#define POT_PIN 25

int delayTime;
int potVal;

void move(void *pvParameters){
  for(;;){
    digitalWrite(STEP_PIN, HIGH);
    vTaskDelay(delayTime / (portTICK_PERIOD_MS * 1000));
    digitalWrite(STEP_PIN, LOW);
    vTaskDelay(delayTime / (portTICK_PERIOD_MS * 1000));
  }
}

void print(void *pvParameters){
  for(;;){
    Serial.print(delayTime);
    Serial.print(" us");
    Serial.print("  Core");
    Serial.println(xPortGetCoreID());
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void setDelayFromPot(void *pvParameters){
  for(;;){
    potVal = analogRead(POT_PIN);
  
    Serial.print(potVal);
    Serial.print("  Core");
    Serial.println(xPortGetCoreID());
    delayTime = map (potVal, 0, 4095, 100, 1000);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}






void setup() {
  // put your setup code here, to run once:
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    move,
    "motor",
    10000,
    NULL,
    1,
    NULL,
    0
  );

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

