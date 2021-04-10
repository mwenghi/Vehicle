#include "Vehicle.h"

Vehicle::Vehicle()
{
  _blinkerStatus = BLINK_OFF;
}

void Vehicle::init()
{
  _lights.init();
  _receiver.init();
}

void Vehicle::iterate()
{
  _lights.iterate();
  _receiver.iterate();

  char user_input;   

  if (Serial.available()) {
     
    user_input = Serial.read(); 
    
    if (user_input =='1')
    {
      _lights.offLights();
    }
    else if(user_input =='2')
    {
      _lights.parkingLights();
    }
    else if(user_input =='3')
    {
      _lights.normalLights();
    }
    else if(user_input =='4')
    {
      _lights.distanceLights();
    }
    else if(user_input =='a')
    {
      if (_blinkerStatus == BLINK_ALERT) {
        _lights.stopBlinkersAlert();
        _blinkerStatus = BLINK_OFF;
      } else {
        _lights.startBlinkersAlert();
        _blinkerStatus = BLINK_ALERT;
      }
    }
    else if(user_input ==',')
    {
      if (_blinkerStatus == BLINK_LEFT) {
        _lights.stopLeftBlinker();
        _blinkerStatus = BLINK_OFF;
      } else {
        _lights.startLeftBlinker();
        _blinkerStatus = BLINK_LEFT;
      }
    }
    else if(user_input =='.')
    {
      if (_blinkerStatus == BLINK_RIGHT) {
        _lights.stopRightBlinker();
        _blinkerStatus = BLINK_OFF;
      } else {
        _lights.startRightBlinker();
        _blinkerStatus = BLINK_RIGHT;
      }
    }
    else
    {
      Serial.println("Invalid option entered.");
    }
  }
}
