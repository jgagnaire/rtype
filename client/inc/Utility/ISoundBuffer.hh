#ifndef ISOUNDBUFFER_HH_
# define ISOUNDBUFFER_HH_

#include <cstdint>

class ISoundBuffer
{
public:
virtual ~ISoundBuffer() {}

  virtual uint64_t getDuration() const = 0;
  virtual uint64_t getSampleCount() const = 0;
  virtual unsigned int getSampleRate() const = 0;
  virtual const short int *getSamples() const = 0;
	virtual bool	loadFromFile(const std::string &file) = 0;
  virtual bool loadFromSamples (const short int *samples, uint64_t sampleCount,
			unsigned int channelCount, unsigned int sampleRate) = 0;

};

#endif // !ISOUNDBUFFER_HH_
