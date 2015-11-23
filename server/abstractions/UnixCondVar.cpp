#include "UnixCondVar.hh"

UnixCondVar::UnixCondVar() {
  pthread_mutex_init(&_mutex, 0);
  pthread_cond_init(&_cond, 0);
}

UnixCondVar::~UnixCondVar() {
  pthread_mutex_destroy(&_mutex);
  pthread_cond_destroy(&_cond);
}

void UnixCondVar::wait(void) {
  if (pthread_mutex_lock(&_mutex) == -1)
    throw(MutexException("Lock failed"));
  if (pthread_cond_wait(&_cond, &_mutex) == -1)
    throw(MutexException("Wait failed"));
  if (pthread_mutex_unlock(&_mutex) == -1)
    throw(MutexException("Unlock failed"));
}

void UnixCondVar::signal(void) {
  if (pthread_cond_signal(&_cond) == -1)
    throw(MutexException("Signal failed"));
}

void UnixCondVar::broadcast(void) {
  if (pthread_cond_broadcast(&_cond) == -1)
    throw(MutexException("Broadcast failed"));
}
