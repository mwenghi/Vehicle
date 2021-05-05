#include "Receiver.h"

/**
 * Construct the receiver
 */
Receiver::Receiver(uint8_t receiverPin, uint8_t channelsCount, uint8_t filterSampleSize)
{
  _receiverPin = receiverPin;
  _channelsCount = channelsCount;
  _filterSampleSize = filterSampleSize;
}

void Receiver::init()
{
  _receiver.begin(_receiverPin, sbusNonBlocking);
  _loop.start(REFRESH_RATE);
}


void Receiver::iterate()
{

  _handleSwitches();
  
  if (_loop.justFinished()) {
    
    _loop.repeat();
    _updateValues();
//    _debugPrint();


  }
}

void Receiver::setSwitchStateHandler(SwitchStateHandler* switchStateHandler)
{
  _switchStateHandler = switchStateHandler;
}
    
uint8_t Receiver::getValue(uint8_t channel)
{
  return map(_medianValues[channel], 988, 2012, 0, 255);
}

bool Receiver::signalLossActive()
{

  return _receiver.signalLossActive();
}


bool Receiver::failsafeActive()
{

  return _receiver.failsafeActive();
}
//
//
//void Receiver::onChannelChange(uint8_t channel, void (*callback)(SwitchState))
//{
//
//  _callbacks[channel] = callback;
//}

/**
 * Get "raw" channel value from receiver
 */
uint16_t Receiver::_getChannelValue(uint8_t channelNumber)
{
  return _receiver.getChannel(channelNumber);
}

/**
 * Returns true if channel was changed
 */
bool Receiver::_isChannelDirty(uint8_t channel)
{
  return _medianValues[channel] != _oldMedianValues[channel];
}

/**
 * Read fresh value and shuffle the buffer
 */
void Receiver::_updateValues()
{
  for (uint8_t channel = 0; channel < _channelsCount; channel++) {

    uint16_t valsForFilter[_filterSampleSize] = {_getChannelValue(channel + 1)};
      
    // move buffer for channel one position down
    for (uint8_t buffer = 1; buffer < _filterSampleSize; buffer++) {

      _values[channel][buffer] = _values[channel][buffer - 1];
      valsForFilter[buffer] = _values[channel][buffer];

    }

    // store fresh value to begin of channel buffer
    _values[channel][0] = valsForFilter[0];

    // store median value from actual buffer 
    _oldMedianValues[channel] = _medianValues[channel];
    _medianValues[channel] = QuickMedian<uint16_t>::GetMedian(valsForFilter, _filterSampleSize);
    
//    _values[channel][0] = _medianValues[channel];
  }
}

void Receiver::_initSwitchStates()
{
  int anticycler = 10000;
  bool confirmed = true;

  do {

    for (uint8_t sw = 4; sw < CHANNELS_COUNT; sw++) {

      SwitchState value = _switchStatusByChannel(sw);

      if (value != _switchStatus[sw]) { // each channel must have same value minimal 2times in a row to confirm the initial state

         confirmed = false;
      }
      
      _switchStatus[sw] = value;
      
    }    
  
  } while (!confirmed || --anticycler > 0);

}


SwitchState Receiver::_switchStatusByChannel(uint8_t channel)
{

  uint8_t value = getValue(channel);
  
  if (value >= SWITCH_POSITION_HIGH_MIN) {

    return SWITCH_HIGH;
    
  } else if (value >= SWITCH_POSITION_MIDDLE_MIN) {

    return SWITCH_MIDDLE;
    
  } else {

    return SWITCH_LOW;
    
  }
}


void Receiver::_handleSwitches()
{

  for (uint8_t sw = 4; sw < CHANNELS_COUNT; sw++) {

    // handle just finished timers
    if ( _switchStartTimeout[sw].isRunning() 
      && _switchStartTimeout[sw].justFinished()) {

      _switchStatus[sw] = _switchChangeStatus[sw];
      _handleSwitchesChange(sw, _switchStatus[sw]);
    }

    // read actal value
    SwitchState actual_value = _switchStatusByChannel(sw);

    // stop running timers if the value was change while running
    if (_switchChangeStatus[sw] != actual_value
        && _switchStartTimeout[sw].isRunning()) {

      _switchStartTimeout[sw].stop();
    }

    // run switch change timers for changed channels
    if (_switchStatus[sw] != actual_value) {

      if (! _switchStartTimeout[sw].isRunning()) {
        
        _switchChangeStatus[sw] = actual_value;
        _switchStartTimeout[sw].start(SWITCH_CHANGE_TIMEOUT);
      }
    }
  }  
}



void Receiver::_handleSwitchesChange(uint8_t channel, SwitchState newState)
{

  if (_switchStateHandler) {
    
    _switchStateHandler->handleSwitch(channel, newState);
  }

//  if (_callbacks[channel]) {
//
//    _callbacks[channel](newState);
//  }
}




void Receiver::_debugPrint()
{
    
    bool hasChange = false;
    for (int i=0; i < _channelsCount; i++) {

//      if (_isChannelDirty(i)) {
        
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(_medianValues[i]); 
        Serial.print("\t");
        hasChange = true;
//      }d
    }
   
      
    if (_receiver.signalLossActive()) {
      Serial.print("SIGNAL_LOSS ");
      hasChange = true;
    }
      
    if (_receiver.failsafeActive()) {
      Serial.print("FAILSAFE");
      hasChange = true;
    }

    if (hasChange) {
      Serial.println(); 
    }
}
