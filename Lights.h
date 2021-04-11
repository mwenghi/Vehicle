#pragma once

/**         L E F T                                 R I G H T 
 *              
 *            LAMP                                     LAMP                           F R O N T
 *  BLINKER    LED    PARKING                PARKING   LED    BLINKER                     
 *     2        1       0                       3       4       5  
 * 
 *  
 *     2        1       0                       3       4       5    
 *  BLINKER    LED    REVERSE                REVERSE   LED    BLINKER             R E A R
 */

#include <FastLED.h>
#include <millisDelay.h>
#include "Module.h"
#include "Log.h"

// PINS
#define FRONT_LAMP_PIN 13
#define FRONT_LED_PIN 14
#define REAR_LED_PIN 12

// LEVELS
#define DISTANCE_LAMP_LEVEL_OFF 0
#define DISTANCE_LAMP_LEVEL_PARKING 0
#define DISTANCE_LAMP_LEVEL_NORMAL 3
#define DISTANCE_LAMP_LEVEL_DISTANCE 100
#define DISTANCE_LAMP_LEVEL_ALERT 100

#define FRONT_LED_LEVEL_OFF 0
#define FRONT_LED_LEVEL_PARKING 0
#define FRONT_LED_LEVEL_PARKING_EMERGENCY 10
#define FRONT_LED_LEVEL_NORMAL 69
#define FRONT_LED_LEVEL_DISTANCE 100

#define REAR_LED_LEVEL_OFF 0
#define REAR_LED_LEVEL_PARKING 0
#define REAR_LED_LEVEL_PARKING_EMERGENCY 3
#define REAR_LED_LEVEL_NORMAL 3
#define REAR_LED_LEVEL_DISTANCE 3
#define REAR_LED_LEVEL_ALERT 100

#define BREAK_LIGHT_LEVEL_OFF 0
#define BREAK_LIGHT_LEVEL_ON 100
#define REVERSE_LIGHT_LEVEL_OFF 0
#define REVERSE_LIGHT_LEVEL_ON 100

#define LEDS_COUNT 6

// INTERVALS
#define BLINKER_INTERVAL 777
#define ALERT_INTERVAL 269

#define FRONT_PARKING_LEFT 0
#define FRONT_PARKING_RIGHT 3
#define FRONT_LED_LEFT 1
#define FRONT_LED_RIGHT 4
#define FRONT_BLINKER_LEFT 2
#define FRONT_BLINKER_RIGHT 5

#define REAR_REVERSE_LEFT 0
#define REAR_REVERSE_RIGHT 3
#define REAR_LED_LEFT 1
#define REAR_LED_RIGHT 4
#define REAR_BLINKER_LEFT 2
#define REAR_BLINKER_RIGHT 5

enum LightStatus {
  MODE_ALERT, MODE_OFF, MODE_PARKING, MODE_NORMAL, MODE_DISTANCE
};

enum LightPosition {
  POSITION_LEFT, POSITION_RIGHT
};

class Lights : public Module {

  public:
  
    Lights();
    Lights(uint8_t frontLampPin, uint8_t frontLedPin, uint8_t rearLedPin);
    
    virtual void init();
    virtual void iterate();

    // PUBLIC INTERFACE
    // alerts
    bool startBlinkersAlert();
    bool stopBlinkersAlert();
    // lights
    bool offLights();
    bool parkingLights();
    bool normalLights();
    bool distanceLights();
    bool alertLights();
    // blinkers
    bool startLeftBlinker();
    bool startRightBlinker();
    bool stopLeftBlinker();
    bool stopRightBlinker();
    bool setLog(Log *debugLog);
    // breaks
    bool startBreaking();
    bool stopBreaking();
    // reverse
    bool startReverse();
    bool stopReverse();

  private:

    Log *_debug;
    LightStatus _status;
    millisDelay _blinkerLoop;
    millisDelay _alertLoop;
    CRGB _frontLeds[LEDS_COUNT];
    CRGB _rearLeds[LEDS_COUNT];
    
    uint8_t _frontLedPin;
    uint8_t _rearLedPin;
    uint8_t _frontLampPin;

    bool _hasChange = true;
    bool _modeBreak = false;
    bool _modeReverse = false;
    bool _modeLeftBlinker = false;
    bool _modeRightBlinker = false;
    bool _modeAlertBlinker = false;
    bool _modeAlertLights = false;
    bool _blinkerState = false;    
    bool _blinkerLightState = false;    
    bool _alertLightState = false;    

    bool _startBlinkerLoop();
    bool _startAlertLoop();
    bool _setLights(LightStatus lightStatus);
    bool _setFrontLamps(LightStatus lightStatus);
    bool _setFrontLight(LightStatus lightStatus);
    bool _setRearLight(LightStatus lightStatus);
    bool _setFrontLamp(LightStatus lightStatus);
    bool _setBreakLight(bool breakStatus, bool onlyOn = false);
    bool _setBreak(bool breakStatus);
    bool _setReverseLight(bool reverseStatus, bool onlyOn = false);
    bool _setReverse(bool reverseStatus);
    bool _updateBreakAndReverse();
    bool _setBlinker(LightPosition lightPosition, bool lightStatus);
    bool _updateBlinkers();
    bool _updateLights();
    bool _resetBlinkerLoop();
    bool _resetAlertLoop();
    bool _handleLightLoops();
    bool _handleBlinkerLoop();
    bool _handleAlertLoop();
    bool _blinkLeft(bool blinkStatus);
    bool _blinkRight(bool blinkStatus);
    bool _blinkAlert(bool blinkStatus);
    bool _lightAlert(bool alertStatus);
    
};
