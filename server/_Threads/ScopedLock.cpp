#include "ScopedLock.hh"

ScopedLock::ScopedLock(IMutex *&mutex) : _mutex(mutex) {
  _mutex->lock();
}

ScopedLock::~ScopedLock() {
  _mutex->unlock();
}
