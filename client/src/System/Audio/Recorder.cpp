#include "Recorder.hh"
#include "Utility/SoundBuffer.hh"

Recorder::Recorder(AudioCallSystem &obj) : SoundRecorder(), _obj(obj)
{
  this->setProcessingInterval(sf::milliseconds(500));
}

Recorder::~Recorder()
{
}

bool Recorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
  SoundBuffer *tmp;

  tmp = new SoundBuffer();
  tmp->loadFromSamples(samples, sampleCount, 2, sampleCount);    
  this->_obj.addPacket(tmp);
  return true;
}
