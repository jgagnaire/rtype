#ifndef RTYPE_ITHREADPOOL_H
#define RTYPE_ITHREADPOOL_H

#include <stddef.h>

template <typename RET_VAL, typename ARG>
class IThreadPool {
public:
    virtual ~IThreadPool() {}
    virtual uint64_t getIdleThreadNumber() const = 0;
    virtual void joinThread(uint64_t id) = 0;
    virtual void joinAll() = 0;
    virtual uint64_t add(RET_VAL(*)(ARG), ARG) = 0;
    virtual void startThread(uint64_t id) = 0;
    virtual void startAll() = 0;
};

#endif
