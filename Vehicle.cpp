#include "Vehicle.h"

Vehicle::Vehicle(bool debug)
{
  _blinkerLeftStatus = false;
  _blinkerRightStatus = false;
  _blinkerAlertStatus = false;
  _debug = debug;
}

void Vehicle::init()
{
  Serial.begin(115200);
  Serial.println("Booting..."); 
  Serial.print("Init lights...");
  _lights.init();
  Serial.println("\t\t[OK]"); 
  Serial.print("Init receiver..."); 
  _receiver.init();
  Serial.println("\t\t[OK]"); 
  Serial.println("Vehicle booted!");
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
    else if(user_input =='5')
    {
      _lights.alertLights();
    }
    else if(user_input =='6')
    {
      _lights.startBreaking();
    }
    else if(user_input =='7')
    {
      _lights.stopBreaking();
    }
    else if(user_input =='8')
    {
      _lights.startReverse();
    }
    else if(user_input =='9')
    {
      _lights.stopReverse();
    }
    else if(user_input =='a')
    {
      if (_blinkerAlertStatus) {
        _lights.stopBlinkersAlert();
        _blinkerAlertStatus = false;
      } else {
        _lights.startBlinkersAlert();
        _blinkerAlertStatus = true;;
      }
    }
    else if(user_input ==',')
    {
      if (_blinkerLeftStatus) {
        _lights.stopLeftBlinker();
        _blinkerLeftStatus = false;
      } else {
        _lights.stopRightBlinker();
        _lights.startLeftBlinker();
        _blinkerLeftStatus = true;
        _blinkerRightStatus = false;
      }
    }
    else if(user_input =='.')
    {
      if (_blinkerRightStatus) {
        _lights.stopRightBlinker();
        _blinkerRightStatus = false;
      } else {
        _lights.stopLeftBlinker();
        _lights.startRightBlinker();
        _blinkerRightStatus = true;
        _blinkerLeftStatus = false;
      }
    }
    else
    {
      Serial.println("Invalid option entered.");
    }
  }
}
