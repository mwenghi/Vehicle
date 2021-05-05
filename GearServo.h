#pragma once

#include <Servo.h>
#include "Module.h"

#define SERVO_PIN 50

#define SERVO_ANGLE_NEUTRAL 85
#define SERVO_ANGLE_GEAR_ONE 65
#define SERVO_ANGLE_GEAR_TWO 105

enum GearServoPosition {
  POSITION_NEUTRAL, POSITION_GEAR_ONE, POSITION_GEAR_TWO
};

class GearServo : public Module 
{

  public:

    GearServo();
    GearServo(uint8_t servoPin);
    
    void init();
    void iterate();
    bool setToNeutral();
    bool setToGearOne();
    bool setToGearTwo();


  private:

    Servo _servo;
    uint8_t _servoPin;
    bool _setPosition(GearServoPosition);
    uint8_t _positionToAngle(GearServoPosition servoPosition);
};
