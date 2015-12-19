#ifndef IRTIME_HH_
#define IRTIME_HH_

#include <cstdint>
#include "Utility/SoundBuffer.hh"

class IRTime
{
public:
	IRTime() {};
	IRTime(sf::Time) {};
  virtual ~IRTime() {}

  virtual std::size_t getTimeMicro() const = 0;
  virtual IRTime &operator=(const ISoundBuffer &rhs) = 0;
};

#endif /* !IRTIME_HH_ */
