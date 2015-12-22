#ifndef RTYPE_THREADPOOL_H
#define RTYPE_THREADPOOL_H

#include <vector>
#include <utility>
#include "ThreadFactory.hh"
#include "Enum.hh"
#include "IThreadPool.hh"

template <typename RET_VAL, typename ARG>
class ThreadPool : public IThreadPool<RET_VAL, ARG> {
private:
    static  uint64_t                              _id;
public:
    ThreadPool() {
        _id = 0;
    }

    virtual ~ThreadPool() {}

    virtual uint64_t getIdleThreadNumber() const {
        uint64_t i = 0;
        for (auto it = _threads.begin(); it != _threads.end(); ++it) {
            i += (it->first.second->state() == Enum::NIL);
        }
        return (i);
    }

    virtual void joinThread(uint64_t id) {
        for (auto it = _threads.begin(); it != _threads.end();) {
            if (it->first.first == id) {
                it->first.second->join();
                _threads.erase(it++);
                return ;
            }
        }
    }

    virtual uint64_t add(RET_VAL(*f)(ARG), ARG arg) {
        IThread<RET_VAL, ARG>  *thr = ThreadFactory::create<RET_VAL, ARG>();
        thr->loadFunc(f);
        _threads.push_back(std::make_pair(std::make_pair(_id, thr), arg));
        return (_id++);
    }

    virtual void joinAll() {
        for (auto it = _threads.begin(); it != _threads.end();) {
            it->first.second->join();
            _threads.erase(it++);
        }
    }

    virtual void startThread(uint64_t id) {
        for (auto it = _threads.begin(); it != _threads.end(); ++it) {
            if (it->first.first == id &&
                it->first.second->state() != Enum::RUNNING)
                it->first.second->create(it->second);
        }
    }

    virtual void startAll() {
        for (auto it = _threads.begin(); it != _threads.end(); ++it) {
            if (it->first.second->state() != Enum::RUNNING)
                it->first.second->create(it->second);
        }
    }

    uint64_t     threadNb() { return (_threads.size()); }
    std::vector<std::pair<std::pair<uint64_t, IThread<RET_VAL, ARG> *>, ARG >> _threads;
};

template <typename RET_VAL, typename ARG>
uint64_t  ThreadPool<RET_VAL, ARG>::_id = 0;

#endif //RTYPE_THREADPOOL_H
