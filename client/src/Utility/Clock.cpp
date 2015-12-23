#include "Utility/Clock.hh"

Clock::Clock()
{}

Clock::~Clock()
{}

uint64_t     Clock::getElapsedTimeMicro() const
{
    return static_cast<uint64_t>(_clock.getElapsedTime().asMicroseconds());
}

uint64_t     Clock::getElapsedTimeMilli() const
{
    return _clock.getElapsedTime().asMilliseconds();
}

void            Clock::restart()
{
    _clock.restart();
}
