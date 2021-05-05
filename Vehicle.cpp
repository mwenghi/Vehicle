#include "Vehicle.h"

Vehicle::Vehicle(bool debug)
{

  _blinkerLeftStatus = false;
  _blinkerRightStatus = false;
  _blinkerAlertStatus = false;
  _debug = debug;
  _motorStatus = false;
  
}

void Vehicle::init()
{
  
  Serial.begin(115200);
  Serial.println("Booting..."); 

  Serial.print("Init sound..."); 
  _sound.init();
  Serial.println("\t\t[OK]"); 
  
  Serial.print("Init lights...");
  _lights.init();
  Serial.println("\t\t[OK]"); 
  
  Serial.print("Init motor..."); 
  _motor.init();
  Serial.println("\t\t[OK]"); 
    
  Serial.print("Init receiver...");
  _receiver.setSwitchStateHandler(this); 
  _receiver.init();
  Serial.println("\t\t[OK]"); 

  Serial.print("Init horn..."); 
  _horn.init();
  Serial.println("\t\t[OK]"); 
  
  Serial.print("Init gear servo..."); 
  _gearServo.init();
  Serial.println("\t\t[OK]"); 
  
  Serial.print("Init sensor..."); 
  _sensor.init();
  Serial.println("\t\t[OK]"); 

  Serial.print("Init stear servo..."); 
  _stearServo.init();
  _stearServo.setPosition(STEAR_ANGLE_NEUTRAL);
  Serial.println("\t\t[OK]"); 

  _sound.startup();
  
  Serial.println("Vehicle booted!");
}


void Vehicle::handleSwitch(uint8_t switchNum, SwitchState newState)
{
  
  _handleSwitchesChange(switchNum, newState);
}


void Vehicle::iterate()
{
  
  _debugFromInput();

  _handleReceiver();
  _handleMotor();
  _handleStear();
  
  _lights.iterate();
  _receiver.iterate();
  _sound.iterate();
  _sensor.iterate();
  _stearServo.iterate();
  _gearServo.iterate();
  _motor.iterate();
  _horn.iterate();

  // just for fun - remove after while
  if (_songDelayTimer.justFinished()) {

    _sound.song();
  }  
}



/**
 * Change connection state by receiver failsafe state
 */
void Vehicle::_handleReceiver()
{

  _receiverConnected = ! (_receiver.failsafeActive() || _receiver.signalLossActive());
}


/**
 * Move wheels left and right
 */
void Vehicle::_handleStear()
{

  _stearServo.setPosition(_receiver.getValue(STEAR_CHANNEL));
}


/**
 * Set speed of motor based on throttle value of receiver
 */
void Vehicle::_handleMotor()
{

  if (_armed && _motorStatus) {
      
    uint8_t motor_pwm = _receiver.getValue(THROTTLE_CHANNEL);
  
    if (motor_pwm > MIN_THROTTLE) {

      uint8_t real_val = map(motor_pwm, MIN_THROTTLE, 255, MAP_MIN_THROTTLE_FROM, 255);
      
      if (_motorDirection) {
        
        _motor.goForward(real_val);
      
      } else {
        
        _motor.goReverse(real_val);
      
      }
  
    } else {

      _motor.setSpeed(0);
    }
        
  } else {

    _stopMotor();
  }
  
}


void Vehicle::_setVehicleState(VehicleState newState)
{
  _status = newState;
}


void Vehicle::_startMotor()
{

  if (!_motorStatus && _prearmCheck()) {
    
    _motor.enable();
    _songDelayTimer.start(2000);
    _motorStatus = true;
    
    _sound.motorStart();
  }
}


void Vehicle::_stopMotor()
{

//  _sound.motorStop();
  _motor.disable();
  _motorStatus = false;
}

bool Vehicle::_prearmCheck()
{

  _armed = _receiverConnected
        && _belowMinThrottle();

  return _armed;
}

bool Vehicle::_belowMinThrottle()
{

  return _receiver.getValue(THROTTLE_CHANNEL) <= SAFETY_MIN_THROTTLE;
}

// TODO: register as event callback
void Vehicle::_handleSwitchesChange(uint8_t channel, SwitchState newState)
{

  if (!_receiverConnected) {

    return;
  }

  switch (channel) {

    case GEAR_LEVEL_CHANNEL:
        _onGearLevelChange(newState);
        break;
    
    case MOTOR_DIRECTION_CHANNEL:
        _onMotorDirectionChange(newState);
        break;
    
    case VEHICLE_SWITCH_CHANNEL:
        _onMotorStatusChange(newState);
        break;
        
    case LIGHT_STATUS_CHANNEL:
        _onLightStatusChange(newState);
        break;
        
    case BLINKER_STATUS_CHANNEL:
        _onBlinkerStatusChange(newState);
        break;
        
    case HORN_CHANNEL:
        _onHornStatusChange(newState);
        break;
    
  }
  
}


void Vehicle::_onMotorStatusChange(SwitchState newState)
{
  
  switch (newState) {

    case SWITCH_LOW:
      
      _stopMotor();
      break;
      
    case SWITCH_HIGH:
      
      _startMotor();
      break;
  
  }
  
}


void Vehicle::_onMotorDirectionChange(SwitchState newState)
{

    switch (newState) {

    case SWITCH_LOW:
      
      _motorDirection = false;
      break;
      
    case SWITCH_HIGH:

      _motorDirection = true;
      break;
  
  }
}

void Vehicle::_onGearLevelChange(SwitchState newState)
{

  switch (newState) {

    case SWITCH_LOW:
      _gearServo.setToNeutral();
      break;
      
    case SWITCH_MIDDLE:
      _gearServo.setToGearOne();
      break;
      
    case SWITCH_HIGH:
      _gearServo.setToGearTwo();
      break;
  }
}


void Vehicle::_onLightStatusChange(SwitchState newState)
{

  switch (newState) {

    case SWITCH_LOW:
      _lights.offLights();
      break;
      
    case SWITCH_MIDDLE:
      _lights.normalLights();
      break;
      
    case SWITCH_HIGH:
      _lights.distanceLights();
      break;
  }
}


void Vehicle::_onBlinkerStatusChange(SwitchState newState)
{

  switch (newState) {

    case SWITCH_LOW:
      _lights.startLeftBlinker();
      break;
      
    case SWITCH_MIDDLE:
      _lights.stopLeftBlinker();
      _lights.stopRightBlinker();
      break;
      
    case SWITCH_HIGH:
      _lights.startRightBlinker();
      break;
  }
}



void Vehicle::_onHornStatusChange(SwitchState newState)
{

  switch (newState) {

    case SWITCH_HIGH:
      _sound.horn();
      break;
            
    case SWITCH_LOW:
      _sound.stop();
      break;
  }
}




void Vehicle::_debugFromInput()
{
  
  
  char user_input;   

  if (Serial.available()) {
     
    user_input = Serial.read(); 

    // lights
    if (user_input =='1')
    {
      _lights.offLights();
    }
    else if(user_input =='2')
    {
      _lights.parkingLights();
    }
    else if(user_input =='3')
    {
      _lights.normalLights();
    }
    else if(user_input =='4')
    {
      _lights.distanceLights();
    }
    else if(user_input =='5')
    {
      
      _sound.alarm();
      _lights.alertLights();
      _lights.startBlinkersAlert();
    }
    else if(user_input =='6')
    {
      _lights.startBreaking();
    }
    else if(user_input =='7')
    {
      _lights.stopBreaking();
    }
    else if(user_input =='8')
    {
      _lights.startReverse();
    }
    else if(user_input =='9')
    {
      _lights.stopReverse();
    }

    // horn
    else if(user_input =='r')
    {
      _horn.startHorning();
    }
    else if(user_input =='t')
    {
      _horn.stopHorning();
    }

    // servo 
    else if(user_input =='q')
    {
      _gearServo.setToNeutral();
    }
    else if(user_input =='w')
    {
      _gearServo.setToGearOne();
    }
    else if(user_input =='e')
    {
      _gearServo.setToGearTwo();
    }

    // blinkers
    else if(user_input =='a')
    {
      if (_blinkerAlertStatus) {
        _lights.stopBlinkersAlert();
        _blinkerAlertStatus = false;
      } else {
        _lights.startBlinkersAlert();
        _blinkerAlertStatus = true;
      }
    }
    else if(user_input ==',')
    {
      if (_blinkerLeftStatus) {
        _lights.stopLeftBlinker();
        _blinkerLeftStatus = false;
      } else {
        _lights.stopRightBlinker();
        _lights.startLeftBlinker();
        _blinkerLeftStatus = true;
        _blinkerRightStatus = false;
      }
    }
    else if(user_input =='.')
    {
      if (_blinkerRightStatus) {
        _lights.stopRightBlinker();
        _blinkerRightStatus = false;
      } else {
        _lights.stopLeftBlinker();
        _lights.startRightBlinker();
        _blinkerRightStatus = true;
        _blinkerLeftStatus = false;
      }
    }


    else if(user_input =='z')
    {
      _motor.disable();
      Serial.println("Motor disabled");
    }

    else if(user_input =='x')
    {
      _motor.enable();
      Serial.println("Motor enabled");
    }

    else if(user_input =='c')
    {
      _motor.forward();
      Serial.println("Motor forward");
    }
    
    else if(user_input =='v')
    {
      _motor.reverse();
      Serial.println("Motor reverse");
    }
    
    else if(user_input =='b')
    {
      _motor.goForward(_motorSpeed);
      Serial.println("Motor go forward");
    }
    
    else if(user_input =='n')
    {
      _motor.goReverse(_motorSpeed);
      Serial.println("Motor go reverse");
    }
    
    else if(user_input =='m')
    {
      _motor.stop();
      Serial.println("Motor stop");
    }
    
    else if(user_input =='+')
    {
      _motorSpeed++;
      if (_motorSpeed > 255) {
        _motorSpeed = 255;
      }
      
      _motor.setSpeed(_motorSpeed);
      
      Serial.print("(+) Motor speed: ");
      Serial.println(_motorSpeed);
    }
    else if(user_input =='-')
    {

      _motorSpeed--;
      if (_motorSpeed < 0) {
        _motorSpeed = 0;
      }

      _motor.setSpeed(_motorSpeed);

      Serial.print("(-) Motor speed: ");
      Serial.println(_motorSpeed);
    }


    else if(user_input =='l')
    {
      _stearPosition += 10;
      if (_stearPosition > 255) {
        _stearPosition = 255;
      }
      
      _stearServo.setPosition(_stearPosition);
      
      Serial.print("(+) Stear position: ");
      Serial.println(_stearPosition);
    }
    else if(user_input =='k')
    {

      _stearPosition -= 10;
      if (_stearPosition < 0) {
        _stearPosition = 0;
      }

      _stearServo.setPosition(_stearPosition);

      Serial.print("(-) Stear position: ");
      Serial.println(_stearPosition);
    }
    else if(user_input =='u')
    {
      _sound.horn();
    }
    else if(user_input =='i')
    {
      _sound.horn2();
    }
    else if(user_input =='o')
    {
      _sound.alarm();
    }
    else if(user_input =='p')
    {
      _sound.motorStart();
    }

    else if(user_input =='[')
    {
      _sound.ticks();
    }
    
    else if(user_input ==']')
    {
      _sound.startup();
    }
    
    else if(user_input =='=')
    {
      _sound.song();
    }

    else if(user_input =='g')
    {
      _sound.play(1);
    }

    else if(user_input =='h')
    {
      _sound.next();
    }
    
    else if(user_input =='j')
    {
      _sound.stop();
    }

    else if(user_input =='f')
    {
      _sound.info();
    }

    else if(user_input =='s')
    {
      _sound.volumeUp();
    }

    else if(user_input =='d')
    {
      _sound.volumeDown();
    }

    
    else
    {
      Serial.println("Invalid option entered.");
    }
  }
}
