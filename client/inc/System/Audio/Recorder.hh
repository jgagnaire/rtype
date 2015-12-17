#ifndef RECORDER_HH_
# define RECORDER_HH_

#include <iostream>
#include <list>
#include <SFML/Audio.hpp>
#include <SoundBuffer.hh>

class Recorder : public sf::SoundRecorder
{
public:
  Recorder();
  virtual ~Recorder() {}

  inline void changeState()
  {
    _state = !_state;
  }

  SoundBuffer *getBuffer();

private:

  virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);
  std::list<SoundBuffer *> _buffers; 
  bool _state;
};

#endif //!RECORDER_HH_
