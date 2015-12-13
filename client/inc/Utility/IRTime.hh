
#ifndef IRTIME_HH_
#define IRTIME_HH_

#include <cstdint>

class IRTime
{
public:
  virtual ~IRTime() {}

  virtual std::size_t getTimeMicro() const = 0;
  virtual IRTime &operator=(const IRTime &rhs) = 0;
};

#endif /* !IRTIME_HH_ */
