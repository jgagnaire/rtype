#ifndef AUDIOBUFFER_HH_
# define AUDIOBUFFER_HH_

#include <iostream>
#include <list>
#include <SFML/Audio.hpp>

class AudioBuffer
{
public:
  inline AudioBuffer(const std::string &name)
  {
    this->_name = name;
    this->_toDelete = 0;
  }

  ~AudioBuffer();

  inline std::string getName() const
  {
    return (this->_name);
  }

  inline void addBuffer(sf::SoundBuffer *buffer)
  {
    this->_buffers.push_back(buffer);
  }

  inline void popBuffer()
  {
    this->_buffers.pop_front();
  }

  sf::SoundBuffer *getBuffer();
  bool canPlay();
  void play();

private:
  std::string _name;
  std::list <sf::SoundBuffer *> _buffers;
  sf::Sound _sound;
  sf::Clock _clock;
  sf::Time	    _bufferTime;
  sf::SoundBuffer *_toDelete;
};

#endif // !AUDIOBUFFER_HH_
