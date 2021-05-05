#pragma once

#include <Wire.h>
#include "Module.h"
#include "MPU9250.h"
#include <millisDelay.h>

class Sensor : public Module {

  public:

    Sensor();

    void init();
    void iterate();
    

  private:
      
    MPU9250 _sensor;
    void _bootSensor();
    void _printValues();
    millisDelay _readTimer;
    
};
