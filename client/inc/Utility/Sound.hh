#ifndef SOUND_HH_
# define SOUND_HH_

#include <SFML/Audio.hpp>
#include "ISound.hh"

class Sound : public ISound
{
public:
  Sound() {}
  virtual ~Sound() {}

  virtual void play()
  {
    this->_sound.play(); 
  }
  virtual void stop()
  {
    this->_sound.stop();
  }
  virtual void resetBuffer()
  {
    this->_sound.resetBuffer();
  }
  virtual void setBuffer(const ISoundBuffer &buffer)
  {
    this->_buffer.loadFromSamples(buffer.getSamples(), buffer.getSampleCount(),
				  2, buffer.getSampleRate());
    this->_sound.setBuffer(this->_buffer);
  }

  virtual void	setVolume(float vol)
  {
    this->_sound.setVolume(vol);
  }

private:
  sf::Sound	_sound;
  sf::SoundBuffer _buffer;
};

#endif //!SOUND_HH_
