#include "AudioBuffer.hh"

AudioBuffer::~AudioBuffer()
{
}

sf::SoundBuffer *AudioBuffer::getBuffer()
{
  if (this->_buffers.empty())
    return (0);
  return (this->_buffers.front());
}


bool AudioBuffer::canPlay()
{
  if (this->_buffers.empty())
    return (false);
  if (this->_clock.getElapsedTime().asMicroseconds() >= this->_bufferTime.asMicroseconds())
    return (true);
  return (false);
}

void AudioBuffer::play()
{
  static int i = 0;
  sf::SoundBuffer *tmp;

  if (!(tmp = this->getBuffer()))
    return ;
  if (i % 2 == 0)
    {
      this->_sound.resetBuffer();
      this->_sound.setBuffer(*tmp);
      this->_sound.play();
    }
  else
    {
      this->_sound2.resetBuffer();
      this->_sound2.setBuffer(*tmp);
      this->_sound2.play();
    }
  this->_clock.restart();
  _bufferTime = tmp->getDuration();
  if (this->_toDelete)
    delete this->_toDelete;
  this->_toDelete = tmp;
  this->popBuffer();
  ++i;
}
