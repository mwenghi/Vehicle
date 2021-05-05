#pragma once

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "Module.h"

#define START_VOLUME 30
#define VOLUME_MAX 30
#define VOLUME_MIN 0

#define HORN_SONG 5
#define HORN_SONG2 3
#define ALARM_SONG 6
#define TICKS_SONG 4
#define MOTOR_START_SONG 10
#define STARTUP_SONG 2
#define SONG_SONG 1

class Sound : public Module {

  public:

    Sound();
    
    void init();
    void iterate();
    
    void play(int fileNumber);
    void stop();
    void volume(int level);
    void volumeUp();
    void volumeDown();
    
    void next();
    void horn();
    void horn2();
    void ticks();
    void alarm();
    void startup();
    void motorStart();
    void song();
    void info();

  private:

    uint8_t _volume;
    bool _initialized;
    DFRobotDFPlayerMini _player;

    void _setVolume(uint8_t level);
    void _playSong(int fileNumber);
    void _loopSong(int fileNumber);
    void _printDetail(uint8_t type, int value);
    void _printInfo();
};
