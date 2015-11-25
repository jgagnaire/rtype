#include "Recorder.hh"

Recorder::Recorder() : SoundRecorder()
{
  sf::Time interval;

  interval = sf::milliseconds(500);
  this->setProcessingInterval(interval);
}

sf::SoundBuffer *Recorder::getBuffer()
{
  sf::SoundBuffer *tmp;
  if (this->_buffers.empty())
    return (0);
  tmp = this->_buffers.front();
  this->_buffers.pop_front();
  return (tmp);
}
