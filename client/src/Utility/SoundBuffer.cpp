#include "SoundBuffer.hh"

SoundBuffer::SoundBuffer()
{
}

SoundBuffer::~SoundBuffer()
{
}

std::size_t SoundBuffer::getDuration() const
{
  return this->_buffer.getDuration().asMicroseconds();
}

std::size_t SoundBuffer::getSampleCount() const
{
  return this->_buffer.getSampleCount();
}

unsigned int SoundBuffer::getSampleRate() const
{
  return this->_buffer.getSampleRate();
}

const short int *SoundBuffer::getSamples() const
{
  return this->_buffer.getSamples();
}

bool SoundBuffer::loadFromSamples(const short int *samples, std::size_t sampleCount,
				  unsigned int channelCount, unsigned int sampleRate)
{
  return this->_buffer.loadFromSamples(samples, sampleCount, channelCount, sampleRate);
}
