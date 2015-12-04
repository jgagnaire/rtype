#include "Utility/Clock.hh"

Clock::Clock()
{}

Clock::~Clock()
{}

std::size_t     Clock::getElapsedTimeMicro() const
{
    return static_cast<std::size_t>(_clock.getElapsedTime().asMicroseconds());
}

std::size_t     Clock::getElapsedTimeMilli() const
{
    return _clock.getElapsedTime().asMilliseconds();
}

void            Clock::restart()
{
    _clock.restart();
}
