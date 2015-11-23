#include "Recorder.hh"

sf::SoundBuffer *Recorder::getBuffer()
{
  sf::SoundBuffer *tmp;
  if (this->_buffers.empty())
    return (0);
  tmp = this->_buffers.front();
  this->_buffers.pop_front();
  return (tmp);
}
