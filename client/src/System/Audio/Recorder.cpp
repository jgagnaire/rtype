#include "Recorder.hh"

Recorder::Recorder(AudioCallSystem *obj) : SoundRecorder()
{
  this->_obj = obj;
  this->setProcessingInterval(sf::milliseconds(500));
}

Recorder::~Recorder()
{
}

bool Recorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
  sf::SoundBuffer *tmp;

  tmp = new sf::SoundBuffer();
  tmp->loadFromSamples(samples, sampleCount, 2, sampleCount);    
  this->_obj->addPacket(tmp);
  return true;
}
