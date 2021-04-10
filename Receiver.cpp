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


uint8_t Receiver::getValue(uint8_t channel)
{
  return map(_medianValues[channel], 988, 2012, 0, 255);
}

void Receiver::iterate()
{
  if (_loop.justFinished()) {
    
    _loop.repeat();
    _updateValues();
    
//    bool hasChange = false;
//    for (int i=0; i < _channelsCount; i++) {
//
//      if (_isChannelDirty(i)) {
//        
//        Serial.print(i + 1);
//        Serial.print(": ");
//        Serial.print(_medianValues[i]); 
//        Serial.print("\t");
//        hasChange = true;
//      }
//    }
//   
//      
//    if (_receiver.signalLossActive()) {
//      Serial.print("SIGNAL_LOSS ");
//      hasChange = true;
//    }
//      
//    if (_receiver.failsafeActive()) {
//      Serial.print("FAILSAFE");
//      hasChange = true;
//    }
//
//    if (hasChange) {
//      Serial.println(); 
//    }

  }
}

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

    uint16_t valsForFilter[_filterSampleSize];
      
    // move buffer for channel one position down
    for (uint8_t buffer = 1; buffer < _filterSampleSize; buffer++) {

      _values[channel][buffer] = _values[channel][buffer - 1];
      valsForFilter[buffer] = _values[channel][buffer];

    }

    // read fresh value to begin of channel buffer
    _values[channel][0] = _getChannelValue(channel + 1);
    valsForFilter[0] = _values[channel][0];

    // store median value from actual buffer 
    _oldMedianValues[channel] = _medianValues[channel];
    _medianValues[channel] = QuickMedian<uint16_t>::GetMedian(valsForFilter, _filterSampleSize);
  }
}
