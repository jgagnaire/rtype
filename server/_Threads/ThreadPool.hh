#ifndef RTYPE_THREADPOOL_H
#define RTYPE_THREADPOOL_H

#include <vector>
#include "Thread.hh"
#include "Enum.hh"
#include "IThreadPool.hh"

template <typename RET_VAL, typename ARGS>
class ThreadPool : public IThreadPool<ARGS> {
public:
    ThreadPool() {}

    virtual ~ThreadPool() {}

    virtual size_t getIdleThreadNumber() const {
        size_t i = 0;
        for (auto it = _threads.begin(); it != _threads.end(); ++it) {
            i = ((*it)->state() == Enum::RUNNING);
        }
        return (i);
    }

    virtual void joinThread(size_t id) {
        if (id < _threads.size())
            _threads[id]->join();
    }

    virtual size_t newThread(ARGS arg, thread_func f) {
        IThread<RET_VAL, ARGS>  *thr = Thread::create<RET_VAL, ARGS>();
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

    virtual void startAll(size_t id) {
        for (int i = 0; i < _threads.size(); ++i) { _threads[i]->create(_args[i]); }
    }

private:
    std::vector<IThread<RET_VAL, ARGS> *>   _threads;
    std::vector<ARGS>                       _args;
};


#endif //RTYPE_THREADPOOL_H
