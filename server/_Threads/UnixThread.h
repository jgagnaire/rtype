#pragma once
# include <signal.h>
# include <unistd.h>
# include "Exceptions.h"
# include "IThread.h"

template <typename RET_VAL, typename ARG>
class UnixThread : public IThread<RET_VAL, ARG> {
private:
	pthread_t	_thread;
	ThreadState	_state;
	thread_func	_func;
	void		*_arg;
public:
  UnixThread()
    : _state(NONE), _func(0), _arg(0) { }

  virtual ~UnixThread() { }

  static void	*start(void *arg)
  {
    UnixThread<RET_VAL, ARG>	*t;
    void	*ret;
    void	*tmp;

    t = static_cast<UnixThread<RET_VAL, ARG> *>(arg);
    tmp = t->_arg;
    ret = t->_func(tmp);
    return (ret);
  }

  virtual void	create(void *arg)
  {
    _arg = arg;
    if (::pthread_create(&_thread, 0, start, this) == -1)
      throw ThreadException("Cannot create thread");
    _state = RUNNING;
  }

  virtual void	join()
  {
    void		*ret_val;

    if (::pthread_join(_thread, &ret_val) == -1)
      throw ThreadException("Cannot join thread");
    _state = DEAD;
    return (ret_val);
  }

  virtual void	kill(int sig)
  {
    if (::pthread_kill(_thread, sig) == -1)
      throw ThreadException("Cannot kill thread");
    _state = DEAD;
  }

  virtual void	loadFunc(RET_VAL(*p)(ARG))
  {
    _func = reinterpret_cast<thread_func>(p);
  }

  ThreadState	state() const
  {
    return (_state);
  }
};
