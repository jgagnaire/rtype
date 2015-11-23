#ifndef SCOPED_LOCK_HH_
# define SCOPED_LOCK_HH_
# include "Mutex.hh"

class ScopedLock {
public:
  ScopedLock(IMutex *&);
  ~ScopedLock();
private:
  IMutex	*&_mutex;
};

#endif
