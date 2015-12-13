#ifndef RECORDER_HH_
# define RECORDER_HH_

#include <iostream>
#include <list>
#include <SFML/Audio.hpp>
#include "AudioCallSystem.hh"

class AudioCallSystem;

class Recorder : public sf::SoundRecorder
{
public:
  Recorder(AudioCallSystem &obj);
  virtual ~Recorder();
  
  void changeState();

private:

  virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);
  AudioCallSystem &_obj;
  bool _state;
};

#endif //!RECORDER_HH_
