#pragma  once
#include "Thread.h"

template<typename T>
class IThreadPool {
  virtual int	startAll() = 0;
  virtual bool	start(T thread_id) = 0;
  virtual bool	waitAll() = 0;
  virtual bool	waitForThread(T thread_id) = 0;

  template<typename RET_VAL, typename ARG>
  IThreadPool	*operator<<(T thread_id) = 0;
  virtual ~IThreadPool() {}
};
