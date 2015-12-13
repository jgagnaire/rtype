#ifndef ISOUNDBUFFER_HH_
# define ISOUNDBUFFER_HH_

#include <cstdint>

class ISoundBuffer
{
public:
virtual ~ISoundBuffer() {}

  virtual std::size_t getDuration() const = 0;
  virtual std::size_t getSampleCount() const = 0;
  virtual unsigned int getSampleRate() const = 0;
  virtual const short int *getSamples() const = 0;
  virtual bool loadFromSamples (const short int *samples, std::size_t sampleCount,
			unsigned int channelCount, unsigned int sampleRate) = 0;

};

#endif // !ISOUNDBUFFER_HH_
