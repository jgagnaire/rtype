#ifndef SOUNDBUFFER_HH_
# define SOUNDBUFFER_HH_

#include <SFML/Audio.hpp>
#include "ISoundBuffer.hh"

class SoundBuffer : public ISoundBuffer
{
public:
  SoundBuffer();
  virtual ~SoundBuffer();

  virtual std::size_t getDuration() const;
  virtual std::size_t getSampleCount() const;
  virtual unsigned int getSampleRate() const;
  virtual const short int *getSamples() const;
  virtual bool loadFromSamples (const short int *samples, std::size_t sampleCount,
			unsigned int channelCount, unsigned int sampleRate);

private:
  sf::SoundBuffer	_buffer;
};

#endif // !SOUNDBUFFER_HH_
