#include "Lights.h"


Lights::Lights()
{
  
  _frontLedPin = FRONT_LED_PIN;
  _rearLedPin = REAR_LED_PIN;
  _frontLampPin = FRONT_LAMP_PIN;
  
  _modeBreak = false;
  _modeReverse = false;
}


Lights::Lights(uint8_t frontLampPin, uint8_t frontLedPin, uint8_t rearLedPin)
{
  
  _frontLedPin = frontLampPin;
  _rearLedPin = frontLedPin;
  _frontLampPin = rearLedPin;  

  _modeBreak = false;
  _modeReverse = false;
}


void Lights::init()
{
  
  FastLED.addLeds<WS2812B, FRONT_LED_PIN, GRB>(_frontLeds, 6);
  FastLED.addLeds<WS2812B, REAR_LED_PIN, GRB>(_rearLeds, 6);
  
  pinMode(_frontLampPin, OUTPUT);
  
  _startBlinkerLoop();
  offLights();
}


void Lights::iterate()
{
  
  _handleLightLoops();
  FastLED.show();
}


bool Lights::startBlinkersAlert()
{
  
  _blinkAlert(true);  
  return true;
}


bool Lights::stopBlinkersAlert()
{
  
  _blinkAlert(false);
  return true;
}

bool Lights::startLightsAlert()
{
  
  return true;
}


bool Lights::stopLightsAlert()
{
  
  return true;
}


bool Lights::offLights()
{
  
  _setLights(MODE_OFF); 
  return true;
}


bool Lights::parkingLights()
{  
  
  _setLights(MODE_PARKING);
  return true;
}


bool Lights::normalLights()
{
  
  _setLights(MODE_NORMAL); 
  return true;
}


bool Lights::distanceLights()
{
  
  _setLights(MODE_DISTANCE); 
  return true;
}


bool Lights::startLeftBlinker()
{
  
  _modeRightBlinker && stopRightBlinker();  
  _setBlinker(POSITION_LEFT, true);
  return true;
}


bool Lights::startRightBlinker()
{
  
  _modeLeftBlinker && stopLeftBlinker();  
  _setBlinker(POSITION_RIGHT, true);
  return true;
}


bool Lights::stopLeftBlinker()
{
  
  _setBlinker(POSITION_LEFT, false);
  return true;
}


bool Lights::stopRightBlinker()
{
  
  _setBlinker(POSITION_RIGHT, false);
  return true;
}


bool Lights::_setLights(LightStatus lightStatus)
{
  
  _status = lightStatus;
  
  _setFrontLight(lightStatus);
  _setFrontLamp(lightStatus);
  _setRearLight(lightStatus);
  
  return true;
}


bool Lights::_blinkLeft(bool blinkStatus)
{
  
  // switch off in non blink state or in blinker off in his cycle 
  if (!blinkStatus || !_blinkerLightState) {
    
    _frontLeds[FRONT_BLINKER_LEFT] = CRGB::Black;
    _rearLeds[REAR_BLINKER_LEFT] = CRGB::Black;
    
  } else {
    
    _frontLeds[FRONT_BLINKER_LEFT] = CRGB::Orange;
    _rearLeds[REAR_BLINKER_LEFT] = CRGB::Orange;
  }    
}


bool Lights::_blinkRight(bool blinkStatus)
{
  
  // switch off in non blink state or in blinker off in his cycle 
  if (!blinkStatus || !_blinkerLightState) {
    
    _frontLeds[FRONT_BLINKER_RIGHT] = CRGB::Black;
    _rearLeds[REAR_BLINKER_RIGHT] = CRGB::Black;
    
  } else {
    
    _frontLeds[FRONT_BLINKER_RIGHT] = CRGB::Orange;
    _rearLeds[REAR_BLINKER_RIGHT] = CRGB::Orange;
  }    
}


bool Lights::_setBlinker(LightPosition lightPosition, bool lightStatus, bool forAlert = false)
{
  
  _blinkerState = lightStatus;
  _blinkerLightState = lightStatus;

  if (!forAlert) {
    switch(lightPosition) {
  
       case POSITION_LEFT:
          _modeLeftBlinker = lightStatus;
          break;
  
       case POSITION_RIGHT:
          _modeRightBlinker = lightStatus;
          break;
    }
  }

  _resetBlinkerLoop();
  _updateBlinkers();
  
  return true;
}


bool Lights::_updateBlinkers()
{
  
  _blinkLeft(_modeLeftBlinker || _modeAlertBlinker);
  _blinkRight(_modeRightBlinker || _modeAlertBlinker);

  return true;
}


bool Lights::_blinkAlert(bool blinkStatus)
{
  
  _modeAlertBlinker = blinkStatus;
  _blinkerLightState = blinkStatus;
  _blinkerState = blinkStatus;
  
  _resetBlinkerLoop();
  _updateBlinkers();

  return true;
}


bool Lights::_handleBlinkerLoop()
{
  
  if (_blinkerLoop.justFinished()) {
    
    _blinkerLoop.repeat();  
    _blinkerLightState = !_blinkerLightState;    
    _updateBlinkers();    
  }

  return true;
}


bool Lights::_handleAlertLoop()
{  
  
  if (_alertLoop.justFinished()) {
    _alertLoop.repeat();
    
  }
  
  return true;
}


bool Lights::_handleLightLoops()
{
  
  _handleBlinkerLoop();
  _handleAlertLoop();  
  
  return true;
}


bool Lights::_startBlinkerLoop()
{
  
  _blinkerLoop.start(BLINKER_INTERVAL); 
  return true;
}


bool Lights::_resetBlinkerLoop()
{
  
  _blinkerLoop.restart(); 
  return true;
}


bool Lights::_setFrontLight(LightStatus lightStatus)
{
  
  switch(lightStatus) {

    case MODE_ALERT:
    
      // TODO: blink
      break;
        
    case MODE_OFF:
    
      _frontLeds[FRONT_LED_LEFT] = CRGB::Black;
      _frontLeds[FRONT_LED_RIGHT] = CRGB::Black;
      _frontLeds[FRONT_PARKING_LEFT] = CRGB::Black;
      _frontLeds[FRONT_PARKING_RIGHT] = CRGB::Black;
      break;
        
    case MODE_PARKING:
  
      _frontLeds[FRONT_LED_LEFT] = CRGB::Black;
      _frontLeds[FRONT_LED_RIGHT] = CRGB::Black;
      _frontLeds[FRONT_PARKING_LEFT] = CRGB::Gray;
      _frontLeds[FRONT_PARKING_RIGHT] = CRGB::Gray;
      break;
        
    case MODE_NORMAL:
    
      _frontLeds[FRONT_LED_LEFT] = CRGB::Gray;
      _frontLeds[FRONT_LED_RIGHT] = CRGB::Gray;
      _frontLeds[FRONT_PARKING_LEFT] = CRGB::Gray;
      _frontLeds[FRONT_PARKING_RIGHT] = CRGB::Gray;
      break;
        
    case MODE_DISTANCE:
    
      _frontLeds[FRONT_LED_LEFT] = CRGB::White;
      _frontLeds[FRONT_LED_RIGHT] = CRGB::White;
      _frontLeds[FRONT_PARKING_LEFT] = CRGB::White;
      _frontLeds[FRONT_PARKING_RIGHT] = CRGB::White;
      break;
  }
  return true;
}


bool Lights::_setRearLight(LightStatus lightStatus)
{
  
  switch(lightStatus) {

    case MODE_ALERT:
    
      // TODO: blink
      break;
        
    case MODE_OFF:
    
      _rearLeds[REAR_LED_LEFT] = CRGB::Black;
      _rearLeds[REAR_LED_RIGHT] = CRGB::Black;
      _rearLeds[REAR_REVERSE_LEFT] = CRGB::Black;
      _rearLeds[REAR_REVERSE_RIGHT] = CRGB::Black;
      break;
        
    case MODE_PARKING:
    
      _rearLeds[REAR_LED_LEFT] = CRGB(REAR_LED_LEVEL_PARKING_EMERGENCY, 0, 0);
      _rearLeds[REAR_LED_RIGHT] = CRGB(REAR_LED_LEVEL_PARKING_EMERGENCY, 0, 0);
      _rearLeds[REAR_REVERSE_LEFT] = CRGB(REAR_LED_LEVEL_PARKING, 0, 0);
      _rearLeds[REAR_REVERSE_RIGHT] = CRGB(REAR_LED_LEVEL_PARKING, 0, 0);
      break;
        
    case MODE_NORMAL:
  
      _rearLeds[REAR_LED_LEFT] = CRGB(REAR_LED_LEVEL_NORMAL, 0, 0);
      _rearLeds[REAR_LED_RIGHT] = CRGB(REAR_LED_LEVEL_NORMAL, 0, 0);
      _rearLeds[REAR_REVERSE_LEFT] = CRGB(REAR_LED_LEVEL_NORMAL, 0, 0);
      _rearLeds[REAR_REVERSE_RIGHT] = CRGB(REAR_LED_LEVEL_NORMAL, 0, 0);
      break;
        
    case MODE_DISTANCE:
  
      _rearLeds[REAR_LED_LEFT] = CRGB(REAR_LED_LEVEL_DISTANCE, 0, 0);
      _rearLeds[REAR_LED_RIGHT] = CRGB(REAR_LED_LEVEL_DISTANCE, 0, 0);
      _rearLeds[REAR_REVERSE_LEFT] = CRGB(REAR_LED_LEVEL_DISTANCE, 0, 0);
      _rearLeds[REAR_REVERSE_RIGHT] = CRGB(REAR_LED_LEVEL_DISTANCE, 0, 0);
      break;
  }
  
  return true;
}


bool Lights::_setFrontLamp(LightStatus lightStatus)
{
  
  switch(lightStatus) {

    case MODE_ALERT:
    
      // TODO: blink
      break;
        
    case MODE_OFF:
    
      analogWrite(FRONT_LAMP_PIN, DISTANCE_LAMP_LEVEL_OFF);
      break;
        
    case MODE_PARKING:
  
      analogWrite(FRONT_LAMP_PIN, DISTANCE_LAMP_LEVEL_PARKING);
      break;
        
    case MODE_NORMAL:
    
      analogWrite(FRONT_LAMP_PIN, DISTANCE_LAMP_LEVEL_NORMAL);
      break;
        
    case MODE_DISTANCE:
    
      analogWrite(FRONT_LAMP_PIN, DISTANCE_LAMP_LEVEL_DISTANCE);
      break;
  }
  
  return true;
}


bool Lights::_setBreakLight(bool breakStatus)
{
  
   if (breakStatus) {

    _rearLeds[REAR_LED_LEFT] = CRGB(BREAK_LIGHT_LEVEL_ON, 0, 0);
    _rearLeds[REAR_LED_RIGHT] = CRGB(BREAK_LIGHT_LEVEL_ON, 0, 0);    
    
   } else {

    _rearLeds[REAR_LED_LEFT] = CRGB(BREAK_LIGHT_LEVEL_OFF, 0, 0);
    _rearLeds[REAR_LED_RIGHT] = CRGB(BREAK_LIGHT_LEVEL_OFF, 0, 0);   
   }
  
  return true;
}
