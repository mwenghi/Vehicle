#pragma once

#include <Arduino.h>
#include <millisDelay.h>
#include "SwitchStateHandler.h"
#include "Module.h"
#include "Receiver.h"
#include "Motor.h"
#include "Lights.h"
#include "Horn.h"
#include "GearServo.h"
#include "StearServo.h"
#include "Sound.h"
#include "Sensor.h"

#define SAFETY_MIN_THROTTLE 30

#define THROTTLE_CHANNEL 0
#define STEAR_CHANNEL 1

#define VEHICLE_SWITCH_CHANNEL 4
#define MOTOR_DIRECTION_CHANNEL 5
#define GEAR_LEVEL_CHANNEL 6
#define LIGHT_STATUS_CHANNEL 7
#define BLINKER_STATUS_CHANNEL 8
#define HORN_CHANNEL 9
#define VOLUME_CHANNEL 10
#define RSSI_VALUE_CHANNEL 11

#define MOTOR_START_SAFETY_TIMEOUT 300
#define MOTOR_SHUTDOWN_SAFETY_TIMEOUT 30

enum VehicleBlinkerStates {
  BLINK_OFF, BLINK_LEFT, BLINK_RIGHT, BLINK_ALERT
};

enum VehicleState {
  VEHICLE_OFF, VEHICLE_STAND_BY, VEHICLE_ARMED
};



class Vehicle : public SwitchStateHandler {
  
  public:
  
    Vehicle(bool debug);    
    
    virtual void init();
    virtual void iterate();

    void handleSwitch(uint8_t switchNum, SwitchState newState);

  private:

    bool _armed = false;
    bool _debug = false;
    bool _receiverConnected = false;

    VehicleState _status;
    Motor _motor;
    StearServo _stearServo;
    Receiver _receiver;
    Lights _lights;
    Horn _horn;
    GearServo _gearServo;
    Sound _sound;
    Sensor _sensor;
    
    bool _blinkerLeftStatus;
    bool _blinkerRightStatus;
    bool _blinkerAlertStatus;

    bool _motorDirection;
    bool _motorStatus;
    uint8_t _motorSpeed;
    uint8_t _stearPosition;

    millisDelay _motorStartTimer;
    millisDelay _motorShutdownTimer;
    millisDelay _songDelayTimer;

    void _debugFromInput();

    void _onGearLevelChange(SwitchState newState);
    void _onMotorStatusChange(SwitchState newState);
    void _onMotorDirectionChange(SwitchState newState);
    void _onBlinkerStatusChange(SwitchState newState);
    void _onLightStatusChange(SwitchState newState);
    void _onHornStatusChange(SwitchState newState);

    bool _belowMinThrottle();
    bool _prearmCheck();
    void _startMotor();
    void _stopMotor();
    
    void _startAlarm();
    void _stopAlarm();

    void _handleMotor();
    void _handleStear();
    void _handleGear();
    void _handleReceiver();
    
    void _setVehicleState(VehicleState newState);
    void _handleSwitchesChange(uint8_t channel, SwitchState newState);
    
};
