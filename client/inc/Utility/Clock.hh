#ifndef CLOCK_HH_AP24MVRO
#define CLOCK_HH_AP24MVRO

#include <SFML/System.hpp>
#include "Utility/IClock.hh"

class Clock : public IClock
{
    public:
        Clock();
        virtual ~Clock();

        virtual std::size_t     getElapsedTimeMicro() const;
        virtual std::size_t     getElapsedTimeMilli() const;
        virtual void            restart();

    private:
        sf::Clock       _clock;
};

#endif /* end of include guard: CLOCK_HH_AP24MVRO */
