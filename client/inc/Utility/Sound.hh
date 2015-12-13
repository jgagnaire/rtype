#ifndef SOUND_HH_
# define SOUND_HH_

#include <SFML/Audio.hpp>
#include "ISound.hh"

class Sound : public ISound
{
public:
  Sound() {}
  virtual ~Sound() {}

  inline virtual void play()
  {
    this->_sound.play(); 
  }
  inline virtual void stop()
  {
    this->_sound.stop();
  }
  inline virtual void resetBuffer()
  {
    this->_sound.resetBuffer();
  }
  virtual void setBuffer(const ISoundBuffer &buffer)
  {
    this->_buffer.loadFromSamples(buffer.getSamples(), buffer.getSampleCount(),
				  2, buffer.getSampleRate());
    this->_sound.setBuffer(this->_buffer);
  }

private:
  sf::Sound	_sound;
  sf::SoundBuffer _buffer;
};

#endif //!SOUND_HH_
