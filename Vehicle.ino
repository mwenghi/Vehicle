
#include "Arduino.h"
#include <millisDelay.h>
#include "Vehicle.h"


Vehicle vehicle(true);

millisDelay frequencyTimer;
uint32_t iterationCounter = 0;

void setup() {
  Serial.begin(115200);
  vehicle.init();
  frequencyTimer.start(10000);
}

void loop() {
  
  iterationCounter++;
  vehicle.iterate();
  
  if (frequencyTimer.justFinished()) {
    frequencyTimer.repeat();

    Serial.print("CPS: ");
    Serial.println(iterationCounter / 10);

    iterationCounter = 0;
  }
}
