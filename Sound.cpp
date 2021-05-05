#include "Sound.h"


Sound::Sound()
{
  
  _initialized = false;
  _volume = START_VOLUME;
}


void Sound::init()
{

  delay(100);
  Serial3.begin(9600);
  delay(100);
  
  if (_player.begin(Serial3)) {

    _setVolume(_volume); 
    _initialized = true;
  }
  
}


void Sound::iterate()
{
  
}
void Sound::info()
{
  _printInfo();
}


void Sound::play(int fileNumber)
{
  _playSong(fileNumber);
}


void Sound::stop()
{
  _player.stop();
}


void Sound::next()
{
  _player.next();
  _printInfo();
}


void Sound::volume(int level)
{
  _setVolume(map(level, 0, 255, 0, 30));
}
    

void Sound::volumeUp()
{

  if (_volume < VOLUME_MAX) {
    
    _setVolume( ++ _volume);
  }
}


void Sound::volumeDown()
{

  if (_volume > VOLUME_MIN) {

    _setVolume( -- _volume);
  }
}

void Sound::horn()
{

  _loopSong(HORN_SONG);
}


void Sound::horn2()
{

  _playSong(HORN_SONG2);
}


void Sound::alarm()
{

  _loopSong(ALARM_SONG);
}

void Sound::ticks()
{

  _loopSong(TICKS_SONG);
}


void Sound::motorStart()
{
  
  _playSong(MOTOR_START_SONG);
}


void Sound::startup()
{
  
  _playSong(STARTUP_SONG);
}

void Sound::song()
{
  
  _playSong(SONG_SONG);
}


void Sound::_setVolume(uint8_t level)
{
  
  _player.volume(level);
}
    

void Sound::_playSong(int fileNumber)
{
  
  if (_initialized) {
    
//    _player.stop();
    _player.play(fileNumber);
    _printInfo();
  }
}

void Sound::_loopSong(int fileNumber)
{
  
  if (_initialized) {
    
//    _player.stop();
    _player.loop(fileNumber);
    _printInfo();
  }
}







void Sound::_printInfo()
{
  
  Serial.print("Playing: ");
  Serial.println(_player.readCurrentFileNumber());
}


void _printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}
