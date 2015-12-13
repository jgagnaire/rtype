#include "Utility/RTime.hh"

RTime::RTime()
{}

RTime::~RTime()
{}

std::size_t     RTime::getTimeMicro() const
{
    return static_cast<std::size_t>(_time.asMicroseconds());
}

RTime	&RTime::operator=(const RTime &rhs)
{
  if (this != &rhs)
    {
      this->_time = sf::microseconds(rhs.getTimeMicro());
    }
  return *this;
}
