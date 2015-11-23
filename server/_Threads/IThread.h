#pragma once

# include "Enum.h"
# include <iostream>

template <typename RET_VAL, typename ARG>
class IThread
{
public:
  virtual ~IThread() {};
  virtual void create(void *arg) = 0;
  virtual void join() = 0;
  virtual void kill(int sig) = 0;
  virtual IThread &loadFunc(RET_VAL(*p)(ARG)) = 0;
};
