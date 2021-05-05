#include "StearServo.h"


StearServo::StearServo()
{
  _servoPin = STEAR_SERVO_PIN;
}


StearServo::StearServo(uint8_t servoPin)
{
  _servoPin = servoPin;
}


void StearServo::init()
{
  _servo.attach(_servoPin);
}


void StearServo::iterate()
{

}


bool StearServo::setToNeutral()
{
  
  return _setPosition(STEAR_ANGLE_NEUTRAL);
}


void StearServo::setPosition(uint8_t servoPosition)
{
  
  _setPosition(servoPosition);
}



/** PRIVATE METHODS **/

bool StearServo::_setPosition(uint8_t servoPosition)
{
  _servo.write(map(servoPosition, 0, 255, STEAR_ANGLE_LEFT, STEAR_ANGLE_RIGHT));
  return true;
}
