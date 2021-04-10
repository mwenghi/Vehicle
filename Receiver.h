#pragma once

#include "Module.h"
#include <sbus.h>
#include "QuickMedianLib.h"
#include <millisDelay.h>

#ifndef FILTER_SAMPLE_SIZE
#define FILTER_SAMPLE_SIZE 10
#endif

#ifndef RECEIVER_PIN
#define RECEIVER_PIN 10
#endif

#ifndef CHANNELS_COUNT
#define CHANNELS_COUNT 12
#endif

#ifndef RSSI_CHANNEL
#define RSSI_CHANNEL 12
#endif

#ifndef REFRESH_RATE
#define REFRESH_RATE 33
#endif


class Receiver : public Module {

  public:
  
    Receiver(uint8_t receiverPin = RECEIVER_PIN, uint8_t channelsCount = CHANNELS_COUNT, uint8_t filterSampleSize = FILTER_SAMPLE_SIZE);

    virtual void init();
    virtual void iterate();
    uint8_t getValue(uint8_t channel);

  private:

    millisDelay _loop;
    SBUS _receiver;
    uint8_t _receiverPin;
    uint8_t _channelsCount;
    uint8_t _filterSampleSize;
    uint16_t _values[CHANNELS_COUNT][FILTER_SAMPLE_SIZE];
    uint16_t _medianValues[CHANNELS_COUNT];
    uint16_t _oldMedianValues[CHANNELS_COUNT];

    void _updateValues();
    uint16_t _getChannelValue(uint8_t channelNumber);
    bool _isChannelDirty(uint8_t channel);

};
