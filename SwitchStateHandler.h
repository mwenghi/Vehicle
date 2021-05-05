#pragma once

#include "Arduino.h"

enum SwitchState {
  SWITCH_LOW, SWITCH_MIDDLE, SWITCH_HIGH
};

class SwitchStateHandler
{

  public:

    virtual void handleSwitch(uint8_t switchNum, SwitchState newState);
  
};
