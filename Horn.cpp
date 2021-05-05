#include "Horn.h"

Horn::Horn()
{
  _hornPin = HORN_PIN;
}

Horn::Horn(uint8_t hornPin)
{
  _hornPin = hornPin;
}


void Horn::init()
{
  pinMode(_hornPin, OUTPUT);
}


void Horn::iterate()
{
  
}


bool Horn::startHorning()
{
  
  return _setHorning(true);
}

bool Horn::stopHorning()
{
  
  return _setHorning(false);
}


bool Horn::_setHorning(bool horn)
{
  
  if (horn) {
    tone(_hornPin, HORN_FREQUENCY);
  } else {
    noTone(_hornPin);
  }

  return true;
}
