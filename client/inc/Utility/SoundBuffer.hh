#ifndef SOUNDBUFFER_HH_
# define SOUNDBUFFER_HH_

#include <SFML/Audio.hpp>
#include "ISoundBuffer.hh"

class SoundBuffer : public ISoundBuffer
{
public:
  SoundBuffer() {}
  virtual ~SoundBuffer(){}

  inline virtual std::size_t getDuration() const
  {
    return this->_buffer.getDuration().asMicroseconds();
  }
  inline virtual std::size_t getSampleCount() const
  {
    return this->_buffer.getSampleCount();
  }
  inline virtual unsigned int getSampleRate() const
  {
    return this->_buffer.getSampleRate();
  }
  inline virtual const short int *getSamples() const
  {
    return this->_buffer.getSamples();
  }
  inline virtual bool loadFromSamples (const short int *samples, std::size_t sampleCount,
			unsigned int channelCount, unsigned int sampleRate)
  {
    return this->_buffer.loadFromSamples(samples, sampleCount, channelCount, sampleRate);
  }

private:
  sf::SoundBuffer	_buffer;
};

#endif // !SOUNDBUFFER_HH_
