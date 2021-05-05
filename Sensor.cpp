#include "Sensor.h"

Sensor::Sensor()
{
//  _sensor = MPU9250(Wire, 0x68);
}

void Sensor::init()
{  
  
  _bootSensor();
  _readTimer.start(30);
}


void Sensor::iterate()
{  
      
  if (_readTimer.justFinished()) {
    _sensor.readSensor();
    _readTimer.repeat();
//    _printValues();
  }
}

void Sensor::_bootSensor()
{
  
  int status = _sensor.begin();
  if (status < 0) {
    
    Serial.print("IMU FAIL: ");
    Serial.println(status);
  } else {
    
    // setting the accelerometer full scale range to +/-8G 
    _sensor.setAccelRange(MPU9250::ACCEL_RANGE_8G);
    // setting the gyroscope full scale range to +/-500 deg/s
    _sensor.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
    // setting DLPF bandwidth to 20 Hz
    _sensor.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
    // setting SRD to 19 for a 50 Hz update rate
    _sensor.setSrd(19);
  }
}

void Sensor::_printValues()
{

  Serial.print("Acc.:\t");

  Serial.print(("x: " + (String)_sensor.getAccelX_mss()).c_str());
  Serial.print((", y: " + (String)_sensor.getAccelY_mss()).c_str());
  Serial.print((", z: " + (String)_sensor.getAccelZ_mss()).c_str());
  
  Serial.print("\tGyro:\t");

  Serial.print((" " + (String)_sensor.getGyroX_rads()).c_str());
  Serial.print((" " + (String)_sensor.getGyroY_rads()).c_str());
  Serial.print((" " + (String)_sensor.getGyroZ_rads()).c_str());

  Serial.print("\tMag.:\t");

  Serial.print((" " + (String)_sensor.getMagX_uT()).c_str());
  Serial.print((" " + (String)_sensor.getMagY_uT()).c_str());
  Serial.println((" " + (String)_sensor.getMagZ_uT()).c_str());
  
}
