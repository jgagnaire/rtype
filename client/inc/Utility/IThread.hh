#pragma once

# include <iostream>
# include "ClientError.hh"
# include "Enum.hh"

template <typename RET_VAL, typename ARG>
class IThread
{
public:
    typedef RET_VAL	(*thread_func)(ARG);
    virtual ~IThread() {};
    virtual void create(ARG arg) = 0;
    virtual void join() = 0;
    virtual void loadFunc(RET_VAL(*p)(ARG)) = 0;
    virtual Enum::ThreadState state() const = 0;
};
