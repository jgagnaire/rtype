#include "Recorder.hh"

Recorder::Recorder(AudioCallSystem *obj) : SoundRecorder()
{
  sf::Time interval;

  this->_obj = obj;
  interval = sf::milliseconds(500);
  this->setProcessingInterval(interval);
}

Recorder::~Recorder()
{
  this->_buffers.clear();
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
