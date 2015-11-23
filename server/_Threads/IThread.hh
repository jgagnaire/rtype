#pragma once

# include <iostream>
# include "Enum.hh"

typedef void	*(*thread_func)(void *);

template <typename RET_VAL, typename ARG>
class IThread
{
public:
    virtual ~IThread() {};
    virtual void create(void *arg) = 0;
    virtual void join() = 0;
    virtual void kill(int sig) = 0;
    virtual void loadFunc(RET_VAL(*p)(ARG)) = 0;
    Enum::ThreadState	state() const = 0;
};
