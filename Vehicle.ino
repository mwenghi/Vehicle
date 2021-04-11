#include "Vehicle.h"

Vehicle vehicle(true);

void setup() {
  Serial.begin(115200);
  vehicle.init();
}

void loop() {
  vehicle.iterate();
}
