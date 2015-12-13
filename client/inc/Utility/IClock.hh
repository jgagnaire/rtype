#ifndef ICLOCK_HH_ZSYQXLFC
#define ICLOCK_HH_ZSYQXLFC

#include <cstdint>

class IClock
{
    public:
        virtual ~IClock() {}

        virtual std::size_t     getElapsedTimeMicro() const = 0;
        virtual std::size_t     getElapsedTimeMilli() const = 0;
        virtual void            restart() = 0;
};

#endif /* end of include guard: ICLOCK_HH_ZSYQXLFC */
