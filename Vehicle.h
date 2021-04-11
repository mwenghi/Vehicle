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
    Vehicle(bool debug);    
    
    virtual void init();
    virtual void iterate();

  private:
    bool _debug = false;
    Receiver _receiver;
    Lights _lights;
    bool _blinkerLeftStatus;
    bool _blinkerRightStatus;
    bool _blinkerAlertStatus;
  
};
