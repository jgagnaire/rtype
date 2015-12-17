#ifndef RTIME_HH_
#define RTIME_HH_

#include "Utility/IRTime.hh"
#include <SFML/System.hpp>
#include "Utility/IClock.hh"
#include "Utility/SoundBuffer.hh"

class RTime : public IRTime
{
public:
  RTime() {}
  virtual ~RTime() {}

  virtual std::size_t getTimeMicro() const
  {
    return static_cast<std::size_t>(_time.asMicroseconds());
  }
  virtual IRTime	&operator=(const ISoundBuffer &rhs)
  {
    this->_time = sf::microseconds(rhs.getDuration());
    return *this;
  }

private:
  sf::Time       _time;
};

#endif /* !RTIME_HH_ */
