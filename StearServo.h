#pragma once

#include "Arduino.h"
#include "Module.h"
#include <Servo.h>

#define STEAR_SERVO_PIN 8

#define STEAR_ANGLE_LEFT 42
#define STEAR_ANGLE_NEUTRAL 97
#define STEAR_ANGLE_RIGHT 130

class StearServo : public Module
{

  public:

    StearServo();
    StearServo(uint8_t servoPin);
    
    void init();
    void iterate();
    void setPosition(uint8_t servoPosition);
    bool setToNeutral();

  private:

    Servo _servo;
    uint8_t _servoPin;
    bool _setPosition(uint8_t servoPosition);
};
