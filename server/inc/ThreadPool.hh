#ifndef RTYPE_THREADPOOL_H
#define RTYPE_THREADPOOL_H

#include <vector>
#include "ThreadFactory.hh"
#include "Enum.hh"
#include "IThreadPool.hh"

template <typename RET_VAL, typename ARG>
class ThreadPool : public IThreadPool<RET_VAL, ARG> {
public:
    ThreadPool() {}

    virtual ~ThreadPool() {}

    virtual size_t getIdleThreadNumber() const {
        size_t i = 0;
        for (auto it = _threads.begin(); it != _threads.end(); ++it) {
            i += ((*it)->state() != Enum::RUNNING);
        }
        return (i);
    }

    virtual void joinThread(size_t id) {
        if (id < _threads.size())
            _threads[id]->join();
    }

    virtual size_t add(RET_VAL(*f)(ARG), ARG arg) {
        IThread<RET_VAL, ARG>  *thr = ThreadFactory::create<RET_VAL, ARG>();
        thr->loadFunc(f);
        _threads.push_back(thr);
        _args.push_back(arg);
        return (_threads.size() - 1);
    }

    virtual void joinAll() {
        for (auto it = _threads.begin(); it != _threads.end(); ++it) { (*it)->join(); }
    }

    virtual void startThread(size_t id) {
        if (id < _threads.size())
            _threads[id]->create(_args[id]);
    }

    virtual void startAll() {
        for (size_t i = 0; i < _threads.size(); ++i) { _threads[i]->create(_args[i]); }
    }

private:
    std::vector<IThread<RET_VAL, ARG> *>   _threads;
    std::vector<ARG>                       _args;
};


#endif //RTYPE_THREADPOOL_H
