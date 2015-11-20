#pragma once
# include <windows.h>
# include "IThread.h"
# include "Exceptions.h"

typedef void	*(*thread_func)(void *);

template <typename RET_VAL, typename ARG>
class WinThread : public IThread<RET_VAL, ARG>
{
private:
	HANDLE		_thread;
	ThreadState _state;
	thread_func	_func;
	void		*_arg;
public:
  WinThread()
    : _state(NONE), _func(0), _arg(0) { }

  virtual ~WinThread() { }

  static void	*start(LPVOID arg)
  {
    WinThread<RET_VAL, ARG>	*t;
    void	*ret;
    void	*tmp;

    t = static_cast<WinThread<RET_VAL, ARG> *>(arg);
    tmp = t->_arg;
    ret = t->_func(tmp);
    return (ret);
  }

  virtual void	create(void *arg)
  {
    _arg = arg;
    if (!(_thread = ::CreateThread(0, 0,
				   reinterpret_cast<LPTHREAD_START_ROUTINE>(&start),
				   this, 0, 0)))
      throw ThreadException("Cannot create thread");
    _state = RUNNING;
  }

  virtual void	join()
  {
    if (::WaitForSingleObject(_thread, INFINITE) == WAIT_FAILED)
      throw ThreadException("Cannot join thread");
    _state = DEAD;
  }

  virtual void	kill(int exit_code = 0)
  {
    if (::TerminateThread(_thread, exit_code) == 0)
      throw ThreadException("Cannot kill thread");
    _state = DEAD;
  }

  virtual IThread	&loadFunc(RET_VAL(*p)(ARG))
  {
    _func = reinterpret_cast<thread_func>(p);
    return (*this);
  }

  ThreadState	state() const
  {
    return (_state);
  }
};
