#ifndef ISOUND_HH_
# define ISOUND_HH_

#include "Utility/SoundBuffer.hh"

class ISound
{
public:
  virtual ~ISound() {}

  virtual void play() = 0;
  virtual void stop() = 0;
  virtual void resetBuffer() = 0;
  virtual void setBuffer(const ISoundBuffer &buffer) = 0;
};

#endif //!ISOUND_HH_
