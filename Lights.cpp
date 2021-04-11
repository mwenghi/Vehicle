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
  // initialize front leds
  FastLED.addLeds<WS2812B, FRONT_LED_PIN, GRB>(_frontLeds, 6);
  // initialize rear leds
  FastLED.addLeds<WS2812B, REAR_LED_PIN, GRB>(_rearLeds, 6);
  // initialize front lamps
  pinMode(_frontLampPin, OUTPUT);
  
  _startBlinkerLoop();
  _startAlertLoop();
  
  offLights();
}

void Lights::iterate()
{
  
  _handleLightLoops();
  if (_hasChange) {
    FastLED.show();
    _hasChange = false;
  }
}


bool Lights::startBlinkersAlert()
{
  
  return _blinkAlert(true);  
}


bool Lights::stopBlinkersAlert()
{

  return _blinkAlert(false);
}


bool Lights::startBreaking()
{

  return _setBreak(true);
}


bool Lights::stopBreaking()
{

  return _setBreak(false);
}

bool Lights::startReverse()
{

  return _setReverse(true);
}


bool Lights::stopReverse()
{

  return _setReverse(false);
}


bool Lights::offLights()
{
  
  return _setLights(MODE_OFF);
}


bool Lights::parkingLights()
{  
  
  return _setLights(MODE_PARKING);
}


bool Lights::normalLights()
{
   
  return _setLights(MODE_NORMAL);
}


bool Lights::distanceLights()
{
  
  return _setLights(MODE_DISTANCE);
}

bool Lights::alertLights()
{
  
  return _setLights(MODE_ALERT);
}


bool Lights::startLeftBlinker()
{
  
  return _setBlinker(POSITION_LEFT, true);
}


bool Lights::startRightBlinker()
{
  
  return _setBlinker(POSITION_RIGHT, true);
}


bool Lights::stopLeftBlinker()
{
  
  return _setBlinker(POSITION_LEFT, false);
}


bool Lights::stopRightBlinker()
{

  return _setBlinker(POSITION_RIGHT, false);
}



/** PRIVATE METHODS **/

bool Lights::_setLights(LightStatus lightStatus)
{
  
  _status = lightStatus;
  
  _lightAlert(lightStatus == MODE_ALERT);
  
  _setFrontLight(lightStatus);
  _setFrontLamp(lightStatus);
  _setRearLight(lightStatus);
  
  return true;
}


bool Lights::_blinkLeft(bool blinkStatus)
{
  
  _hasChange = true;
  
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
  
  _hasChange = true;
  
  // switch off in non blink state or in blinker off in his cycle 
  if (!blinkStatus || !_blinkerLightState) {
    
    _frontLeds[FRONT_BLINKER_RIGHT] = CRGB::Black;
    _rearLeds[REAR_BLINKER_RIGHT] = CRGB::Black;
    
  } else {
    
    _frontLeds[FRONT_BLINKER_RIGHT] = CRGB::Orange;
    _rearLeds[REAR_BLINKER_RIGHT] = CRGB::Orange;
  }    
}


bool Lights::_setBlinker(LightPosition lightPosition, bool lightStatus)
{
    
  _blinkerState = lightStatus || _modeAlertBlinker;
  _blinkerLightState = lightStatus;

  if (lightPosition == POSITION_LEFT) {
    
    _modeLeftBlinker = lightStatus;
    
  } else if (lightPosition == POSITION_RIGHT) {
   
    _modeRightBlinker = lightStatus; 
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

bool Lights::_updateLights()
{
  
  _setLights(_status);

  return true;
}


bool Lights::_blinkAlert(bool blinkStatus)
{
  
  _modeAlertBlinker = blinkStatus;
  _blinkerLightState = blinkStatus;
  _blinkerState = blinkStatus || _modeLeftBlinker || _modeRightBlinker;
  
  _resetBlinkerLoop();
  _updateBlinkers();

  return true;
}


bool Lights::_lightAlert(bool alertStatus)
{
  
  _modeAlertLights = alertStatus;  
  _resetAlertLoop();
  
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
    _alertLightState = !_alertLightState;
    _updateLights();
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


bool Lights::_startAlertLoop()
{
  
  _alertLoop.start(ALERT_INTERVAL); 
  return true;
}


bool Lights::_resetBlinkerLoop()
{
  
  _blinkerLoop.restart(); 
  return true;
}


bool Lights::_resetAlertLoop()
{
  
  _alertLoop.restart(); 
  return true;
}


bool Lights::_setFrontLight(LightStatus lightStatus)
{
  
  _hasChange = true;
  
  switch(lightStatus) {

    case MODE_ALERT:

      if (_alertLightState) {
        
        _frontLeds[FRONT_LED_LEFT] = CRGB::White;
        _frontLeds[FRONT_LED_RIGHT] = CRGB::White;
        _frontLeds[FRONT_PARKING_LEFT] = CRGB::White;
        _frontLeds[FRONT_PARKING_RIGHT] = CRGB::White;  
        
      } else {
          
        _frontLeds[FRONT_LED_LEFT] = CRGB::Black;
        _frontLeds[FRONT_LED_RIGHT] = CRGB::Black;
        _frontLeds[FRONT_PARKING_LEFT] = CRGB::Black;
        _frontLeds[FRONT_PARKING_RIGHT] = CRGB::Black;
      }
      
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
  
  _hasChange = true;
  
  switch(lightStatus) {

    case MODE_ALERT:
    
      if (_alertLightState) {
          
        _rearLeds[REAR_LED_LEFT] = CRGB(REAR_LED_LEVEL_ALERT, 0, 0);
        _rearLeds[REAR_LED_RIGHT] = CRGB(REAR_LED_LEVEL_ALERT, 0, 0);
        _rearLeds[REAR_REVERSE_LEFT] = CRGB(REAR_LED_LEVEL_ALERT, 0, 0);
        _rearLeds[REAR_REVERSE_RIGHT] = CRGB(REAR_LED_LEVEL_ALERT, 0, 0);
        
      } else {
        
        _rearLeds[REAR_LED_LEFT] = CRGB(REAR_LED_LEVEL_OFF, 0, 0);
        _rearLeds[REAR_LED_RIGHT] = CRGB(REAR_LED_LEVEL_OFF, 0, 0);
        _rearLeds[REAR_REVERSE_LEFT] = CRGB(REAR_LED_LEVEL_OFF, 0, 0);
        _rearLeds[REAR_REVERSE_RIGHT] = CRGB(REAR_LED_LEVEL_OFF, 0, 0);
      }
      
      break;
        
    case MODE_OFF:
    
      _rearLeds[REAR_LED_LEFT] = CRGB(REAR_LED_LEVEL_OFF, 0, 0);
      _rearLeds[REAR_LED_RIGHT] = CRGB(REAR_LED_LEVEL_OFF, 0, 0);
      _rearLeds[REAR_REVERSE_LEFT] = CRGB(REAR_LED_LEVEL_OFF, 0, 0);
      _rearLeds[REAR_REVERSE_RIGHT] = CRGB(REAR_LED_LEVEL_OFF, 0, 0);
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

  _updateBreakAndReverse();
  
  return true;
}


bool Lights::_setFrontLamp(LightStatus lightStatus)
{
  
  switch(lightStatus) {

    case MODE_ALERT:
    
      if (_alertLightState) {
         
        analogWrite(FRONT_LAMP_PIN, DISTANCE_LAMP_LEVEL_ALERT);
        
      } else {
        
        analogWrite(FRONT_LAMP_PIN, DISTANCE_LAMP_LEVEL_OFF);
      }
      
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


bool Lights::_setBreak(bool breakStatus)
{
  
  _modeBreak = breakStatus;
  _setBreakLight(breakStatus);

  return true;
}


bool Lights::_setReverse(bool reverseStatus)
{
  
  _modeReverse = reverseStatus;
  _setReverseLight(reverseStatus);

  return true;
}


bool Lights::_updateBreakAndReverse()
{
  
  _setBreakLight(_modeBreak, true);
  _setReverseLight(_modeReverse, true);

  return true;
}


bool Lights::_setBreakLight(bool breakStatus, bool onlyOn)
{
  
  _hasChange = true;
  
  if (breakStatus) {

    _rearLeds[REAR_LED_LEFT] = CRGB(BREAK_LIGHT_LEVEL_ON, 0, 0);
    _rearLeds[REAR_LED_RIGHT] = CRGB(BREAK_LIGHT_LEVEL_ON, 0, 0);   

    if (!_modeReverse) {
      _rearLeds[REAR_REVERSE_LEFT] = CRGB(BREAK_LIGHT_LEVEL_ON, 0, 0);
      _rearLeds[REAR_REVERSE_RIGHT] = CRGB(BREAK_LIGHT_LEVEL_ON, 0, 0);
    }

  } else if (!onlyOn) {
    
    _setRearLight(_status);
  }
  
  return true;
}


bool Lights::_setReverseLight(bool reverseStatus, bool onlyOn)
{
  
  _hasChange = true;
  
  if (reverseStatus) {

    // 255 shades of gray
    CRGB color = CRGB(REVERSE_LIGHT_LEVEL_ON, REVERSE_LIGHT_LEVEL_ON, REVERSE_LIGHT_LEVEL_ON);
    
    _rearLeds[REAR_REVERSE_LEFT] = color;
    _rearLeds[REAR_REVERSE_RIGHT] = color;

  } else if (!onlyOn) {
    
    _setRearLight(_status);
  }
  
  return true;
}
