#pragma once

#include "Module.h"
#include <sbus.h>
#include "QuickMedianLib.h"
#include "SwitchStateHandler.h"
#include <millisDelay.h>

#ifndef FILTER_SAMPLE_SIZE
#define FILTER_SAMPLE_SIZE 10
#endif

#ifndef RECEIVER_PIN
#define RECEIVER_PIN 11
#endif

#ifndef CHANNELS_COUNT
#define CHANNELS_COUNT 12
#endif

#ifndef RSSI_CHANNEL
#define RSSI_CHANNEL 12
#endif

#ifndef REFRESH_RATE
#define REFRESH_RATE 10
#endif


#define SWITCH_CHANGE_TIMEOUT 69

#define SWITCH_POSITION_LOW_MIN 0
#define SWITCH_POSITION_LOW_MAX 85
#define SWITCH_POSITION_MIDDLE_MIN 86
#define SWITCH_POSITION_MIDDLE_MAX 170
#define SWITCH_POSITION_HIGH_MIN 171
#define SWITCH_POSITION_HIGH_MAX 255

#define MIN_THROTTLE 30
#define MAP_MIN_THROTTLE_FROM 10

class Receiver : public Module {

  typedef void (*callback) (uint8_t, SwitchState);

  public:

    Receiver(uint8_t receiverPin = RECEIVER_PIN, uint8_t channelsCount = CHANNELS_COUNT, uint8_t filterSampleSize = FILTER_SAMPLE_SIZE);

    virtual void init();
    virtual void iterate();

    bool signalLossActive();
    bool failsafeActive();
    uint8_t getValue(uint8_t channel);
    
    void setSwitchStateHandler(SwitchStateHandler* switchStateHandler);
    
    void onChannelChange(uint8_t channel, void (*callback)(uint8_t, SwitchState));

  private:

    SwitchStateHandler *_switchStateHandler;
    SwitchState _switchStatus[CHANNELS_COUNT];
    SwitchState _switchChangeStatus[CHANNELS_COUNT];    
    SwitchState _switchStatusByChannel(uint8_t channel);

    millisDelay _switchStartTimeout[CHANNELS_COUNT];
    millisDelay _loop;
    SBUS _receiver;

    callback _callbacks[CHANNELS_COUNT];
    uint8_t _receiverPin;
    uint8_t _channelsCount;
    uint8_t _filterSampleSize;
    uint16_t _values[CHANNELS_COUNT][FILTER_SAMPLE_SIZE];
    uint16_t _medianValues[CHANNELS_COUNT];
    uint16_t _oldMedianValues[CHANNELS_COUNT];

    void _handleSwitches();
    void _handleSwitchesChange(uint8_t channel, SwitchState newState);
    void _initSwitchStates();
    void _updateValues();
    uint16_t _getChannelValue(uint8_t channelNumber);
    bool _isChannelDirty(uint8_t channel);
    void _debugPrint();

};
