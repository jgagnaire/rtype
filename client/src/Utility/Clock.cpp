#include "Utility/Clock.hh"

Clock::Clock()
{}

Clock::~Clock()
{}

std::size_t     Clock::getElapsedTimeMicro() const
{
    return _clock.getElapsedTime().asMicroseconds();
}

std::size_t     Clock::getElapsedTimeMilli() const
{
    return _clock.getElapsedTime().asMilliseconds();
}

void            Clock::restart()
{
    _clock.restart();
}
