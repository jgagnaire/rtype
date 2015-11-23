#pragma once
# include <signal.h>
# include <unistd.h>
# include <pthread.h>
# include "IThread.hh"

template <typename RET_VAL, typename ARG>
class UnixThread : public IThread<RET_VAL, ARG> {
private:
	pthread_t	                                _thread;
	Enum::ThreadState	                        _state;
	typename IThread<RET_VAL, ARG>::thread_func _func;
	ARG 		                                _arg;
public:
  UnixThread()
    : _state(Enum::NIL), _func(0), _arg(0) { }

  virtual ~UnixThread() { }

  static void	*start(void *arg)
  {
    UnixThread<RET_VAL, ARG>	*t;
    ARG	                        tmp;

    t = static_cast<UnixThread<RET_VAL, ARG> *>(arg);
    tmp = t->_arg;
    t->_func(tmp);
    t->terminateThread();
    return (NULL);
  }

  virtual void	create(ARG arg)
  {
    _arg = arg;
    if (::pthread_create(&_thread, 0, start, this) == -1)
      throw ThreadException("Cannot create thread");
    _state = Enum::RUNNING;
  }

  virtual void	join()
  {
    void		*ret_val;

    if (::pthread_join(_thread, &ret_val) == -1)
      throw ThreadException("Cannot join thread");
    _state = Enum::DEAD;
  }

  virtual void	loadFunc(RET_VAL(*p)(ARG))
  {
    _func = reinterpret_cast<typename IThread<RET_VAL, ARG>::thread_func>(p);
  }

  virtual Enum::ThreadState	state() const
  {
    return (_state);
  }

    void terminateThread() {
      _state = Enum::DEAD;
    }
};
