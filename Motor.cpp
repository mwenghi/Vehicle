#include "Motor.h"


Motor::Motor()
{
  _ena = ENA;
  _ina = INA;
  _inb = INB;
  _cs = CS;
  _pwm = PWM;
}


void Motor::init()
{
  pinMode(_ena, OUTPUT);
  pinMode(_ina, OUTPUT);
  pinMode(_inb, OUTPUT);
  pinMode(_cs, INPUT);
  pinMode(_pwm, OUTPUT);
}


void Motor::iterate()
{
  
}

void Motor::goForward(uint8_t speed)
{
  forward();
  enable();
  setSpeed(speed);
}

void Motor::goReverse(uint8_t speed)
{
  reverse();
  enable();
  setSpeed(speed);
}

void Motor::stop()
{
  disable();
  setSpeed(0);
}


void Motor::enable()
{
  _enableMotor(true);
}


void Motor::disable()
{
  _enableMotor(false);
}


void Motor::forward()
{
  _setDirection(true);
}


void Motor::reverse()
{
  _setDirection(false);
}


void Motor::setSpeed(uint8_t speed)
{
  _setSpeed(speed);
}




bool Motor::_enableMotor(bool enable) 
{

  _enabled = enable;
  
  if (enable) {
   
    digitalWrite(_ena, HIGH); 
    
  } else {
    
    digitalWrite(_ena, LOW); 
  }
  
  return true;
}


bool Motor::_setDirection(bool direction)
{
  _direction = direction;

  if (direction) {
    
    digitalWrite(_ina, LOW);
    digitalWrite(_inb, HIGH);
    
  } else {
      
    digitalWrite(_ina, HIGH);
    digitalWrite(_inb, LOW);
      
  }
  
  return true;
}


bool Motor::_setSpeed(uint8_t speed)
{
  _speed = speed; 
 
  analogWrite(_pwm, speed);
  
  return true;
}

    
