#pragma once

#include "Arduino.h"
#include "Module.h"

#define HORN_PIN 16
#define HORN_FREQUENCY 100

class Horn : public Module 
{

  public:
  
    Horn();
    Horn(uint8_t hornPin);

    void init();
    void iterate();
    
    bool startHorning();
    bool stopHorning();


  private:
    uint8_t _hornPin;
    bool _setHorning(bool horn);
  
};
