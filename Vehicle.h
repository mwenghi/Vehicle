#pragma once

#include <millisDelay.h>
#include "Module.h"
#include "Receiver.h"
#include "Lights.h"

enum VehicleBlinkerStates {
  BLINK_OFF, BLINK_LEFT, BLINK_RIGHT, BLINK_ALERT
};

class Vehicle : public Module 
{

  public:
    Vehicle();    
    
    virtual void init();
    virtual void iterate();

  private:
    Receiver _receiver;
    Lights _lights;
    VehicleBlinkerStates _blinkerStatus;
  
};
