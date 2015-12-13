#include "Recorder.hh"
#include "Utility/SoundBuffer.hh"

Recorder::Recorder(AudioCallSystem &obj) : SoundRecorder(), _obj(obj)
{
  this->_state = false;
  this->setProcessingInterval(sf::milliseconds(500));
}

Recorder::~Recorder()
{
}

bool Recorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
  SoundBuffer *tmp;

  if (_state)
    {
      tmp = new SoundBuffer();
      tmp->loadFromSamples(samples, sampleCount, 2, sampleCount);    
      this->_obj.addPacket(tmp);
    }
  return true;
}

void Recorder::ChangeState()
{
  if (_state)
    _state = false;
  else
    _state = true;
}
