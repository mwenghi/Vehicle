#pragma once

#include "Arduino.h"
#include "Module.h"


#define ENA 32
#define INA 34
#define INB 36
#define CS A14
#define PWM 7

enum MotorDirection {
  DIRECTION_FORWARD, DIRECTION_REVERSE
};

class Motor : public Module {

  public:
    Motor();

    void init();
    void iterate();
    void enable();
    void disable();
    void setSpeed(uint8_t speed);
    void forward();
    void reverse();
    void goForward(uint8_t speed);
    void goReverse(uint8_t speed);
    void stop();

  private:

    uint8_t _ena;
    uint8_t _ina;
    uint8_t _inb;
    uint8_t _cs;
    uint8_t _pwm;

    bool _enabled;
    bool _direction;
    uint8_t _speed;
    
    bool _enableMotor(bool enable);
    bool _setDirection(bool direction);
    bool _setSpeed(uint8_t speed);
    
};
