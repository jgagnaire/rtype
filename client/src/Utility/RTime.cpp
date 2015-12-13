#include "Utility/RTime.hh"

RTime::RTime()
{}

RTime::~RTime()
{}

std::size_t     RTime::getTimeMicro() const
{
    return static_cast<std::size_t>(_time.asMicroseconds());
}

IRTime	&RTime::operator=(const IRTime &rhs)
{
  if (this != &rhs)
    {
      this->_time = sf::microseconds(rhs.getTimeMicro());
    }
  return *this;
}

IRTime	&RTime::operator=(const ISoundBuffer &rhs)
{
  this->_time = sf::microseconds(rhs.getDuration());
  return *this;
}
