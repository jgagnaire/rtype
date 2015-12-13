#ifndef RTIME_HH_
#define RTIME_HH_

#include "Utility/IRTime.hh"
#include <SFML/System.hpp>
#include "Utility/IClock.hh"
#include "Utility/SoundBuffer.hh"

class RTime : public IRTime
{
public:
  RTime();
  virtual ~RTime();

  virtual std::size_t getTimeMicro() const;
  virtual IRTime	&operator=(const IRTime &rhs);
  virtual IRTime	&operator=(const SoundBuffer &rhs);

private:
  sf::Time       _time;
};

#endif /* !RTIME_HH_ */
