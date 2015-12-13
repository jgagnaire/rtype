#ifndef RTIME_HH_
#define RTIME_HH_

#include "Utility/IRTime.hh"
#include <SFML/System.hpp>
#include "Utility/IClock.hh"

class RTime : public IRTime
{
public:
  RTime();
  virtual ~RTime();

  virtual std::size_t getTimeMicro() const;
  virtual RTime	&operator=(const RTime &rhs);

private:
  sf::Time       _time;
};

#endif /* !RTIME_HH_ */
