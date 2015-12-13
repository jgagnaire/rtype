#ifndef SOUND_HH_
# define SOUND_HH_

#include <SFML/Audio.hpp>
#include "ISound.hh"

class Sound : public ISound
{
public:
  Sound();
  virtual ~Sound();

virtual void play();
virtual void stop();
virtual void resetBuffer();
virtual void setBuffer(const SoundBuffer &buffer);

private:
  sf::Sound	_sound;
sf::SoundBuffer _buffer;
};

#endif //!SOUND_HH_
