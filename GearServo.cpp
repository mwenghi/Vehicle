#include "GearServo.h"


GearServo::GearServo()
{
  _servoPin = SERVO_PIN;
}


GearServo::GearServo(uint8_t servoPin)
{
  _servoPin = servoPin;
}


void GearServo::init()
{
  _servo.attach(_servoPin);
}

void GearServo::iterate()
{
  
}


bool GearServo::setToNeutral()
{
  
  return _setPosition(POSITION_NEUTRAL);
}


bool GearServo::setToGearOne()
{
  
  return _setPosition(POSITION_GEAR_ONE);
}


bool GearServo::setToGearTwo()
{
  
  return _setPosition(POSITION_GEAR_TWO);
}



/** PRIVATE METHODS **/

bool GearServo::_setPosition(GearServoPosition servoPosition)
{
  _servo.write(_positionToAngle(servoPosition));
  return true;
}


uint8_t GearServo::_positionToAngle(GearServoPosition servoPosition)
{

  switch(servoPosition) {
    
    case POSITION_GEAR_ONE:
    
      return SERVO_ANGLE_GEAR_ONE;
      
    case POSITION_GEAR_TWO:
    
      return SERVO_ANGLE_GEAR_TWO;
      
    case POSITION_NEUTRAL:
    default:
    
      return SERVO_ANGLE_NEUTRAL;
  }
}
