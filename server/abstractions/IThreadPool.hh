#ifndef RTYPE_ITHREADPOOL_H
#define RTYPE_ITHREADPOOL_H

#include <stddef.h>

template <typename RET_VAL, typename ARG>
class IThreadPool {
public:
    virtual ~IThreadPool() {}
    virtual size_t getIdleThreadNumber() const = 0;
    virtual void joinThread(size_t id) = 0;
    virtual void joinAll() = 0;
    virtual size_t add(RET_VAL(*)(ARG), ARG) = 0;
    virtual void startThread(size_t id) = 0;
    virtual void startAll() = 0;
};

#endif
