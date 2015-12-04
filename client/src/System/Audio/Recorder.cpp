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

bool Recorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
  sf::SoundBuffer *tmp;

  tmp = new sf::SoundBuffer();
  tmp->loadFromSamples(samples, sampleCount, 2, sampleCount);    
  // _buffers.push_back(tmp);
  this->_obj->addPacket(tmp);
  return true;
}
