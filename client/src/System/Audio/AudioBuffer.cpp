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
  sf::SoundBuffer *tmp;

  if (!(tmp = this->getBuffer()))
    return ;
  this->_sound.resetBuffer();
  this->_sound.setBuffer(*tmp);
  this->_sound.play();
  this->_clock.restart();
  _bufferTime = tmp->getDuration();
  if (this->_toDelete)
    delete this->_toDelete;
  this->_toDelete = tmp;
  this->popBuffer();
}
