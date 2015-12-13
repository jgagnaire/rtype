#include "Recorder.hh"
#include "Utility/SoundBuffer.hh"

Recorder::Recorder() : SoundRecorder()
{
  this->_state = true;
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
      this->_buffers.push_back(tmp);
    }
  return true;
}

void Recorder::changeState()
{
  if (_state)
    _state = false;
  else
    _state = true;
}

SoundBuffer *Recorder::getBuffer()
{
  SoundBuffer *tmp;

  if (this->_buffers.empty())
    return 0;
  tmp = this->_buffers.front();
  this->_buffers.pop_front();
  return tmp;
}
