#pragma once
# include <windows.h>
# include "IThread.hh"
# include "Exceptions.h"

template <typename RET_VAL, typename ARG>
class WinThread : public IThread<RET_VAL, ARG>
{
private:
	HANDLE		_thread;
	Enum::ThreadState _state;
	thread_func	_func;
	void		*_arg;
public:
  WinThread()
    : _state(Enum::NIL), _func(0), _arg(0) { }

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
    _state = Enum::RUNNING;
  }

  virtual void	join()
  {
    if (::WaitForSingleObject(_thread, INFINITE) == WAIT_FAILED)
      throw ThreadException("Cannot join thread");
    _state = DEAD;
  }

  virtual void	loadFunc(RET_VAL(*p)(ARG))
  {
    _func = reinterpret_cast<thread_func>(p);
  }

  Enum::ThreadState	state() const
  {
    return (_state);
  }
};
