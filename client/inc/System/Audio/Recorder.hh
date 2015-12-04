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
  Recorder(AudioCallSystem *obj);
  ~Recorder();
  sf::SoundBuffer *getBuffer();

private:
  virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
  {
    sf::SoundBuffer *tmp;

    tmp = new sf::SoundBuffer();
    tmp->loadFromSamples(samples, sampleCount, 2, sampleCount);    
    _buffers.push_back(tmp);
    return true;
  }

  std::list <sf::SoundBuffer *> _buffers;
  AudioCallSystem *_obj;
};

#endif //!RECORDER_HH_
