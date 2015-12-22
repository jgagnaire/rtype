#ifndef SOUNDBUFFER_HH_
# define SOUNDBUFFER_HH_

#include <SFML/Audio.hpp>
#include "ISoundBuffer.hh"

class SoundBuffer : public ISoundBuffer
{
public:
  SoundBuffer() {}
  virtual ~SoundBuffer(){}

  virtual uint64_t getDuration() const
  {
    return static_cast<uint64_t>(this->_buffer.getDuration().asMicroseconds());
  }
  virtual uint64_t getSampleCount() const
  {
    return static_cast<uint64_t>(this->_buffer.getSampleCount());
  }
  virtual unsigned int getSampleRate() const
  {
    return this->_buffer.getSampleRate();
  }
  virtual const short int *getSamples() const
  {
    return this->_buffer.getSamples();
  }
	virtual bool	loadFromFile(const std::string &file)
		{
			return (_buffer.loadFromFile(file));
		}
  virtual bool loadFromSamples (const short int *samples, uint64_t sampleCount,
			unsigned int channelCount, unsigned int sampleRate)
  {
    return this->_buffer.loadFromSamples(samples, sampleCount, channelCount, sampleRate);
  }

private:
  sf::SoundBuffer	_buffer;
};

#endif // !SOUNDBUFFER_HH_
