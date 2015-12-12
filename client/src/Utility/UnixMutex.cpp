#include "UnixMutex.hh"
#include "ClientError.hh"

UnixMutex::UnixMutex()
{
  ::pthread_mutex_init(&_mutex, 0);
}

UnixMutex::~UnixMutex()
{
  ::pthread_mutex_destroy(&_mutex);
}

void	UnixMutex::lock(void)
{
	if (::pthread_mutex_lock(&_mutex) == -1)
		throw MutexException("Lock failed");
}

void	UnixMutex::unlock(void)
{
	if (::pthread_mutex_unlock(&_mutex) == -1)
		throw MutexException("Unlock failed");
}

bool	UnixMutex::trylock(void)
{
	return (!::pthread_mutex_trylock(&_mutex));
}

bool	UnixMutex::timedlock(const struct timespec &t)
{
	int			ret_val;
	struct timespec	timeout;

	::clock_gettime(CLOCK_REALTIME, &timeout);
	timeout.tv_sec += t.tv_sec;
	timeout.tv_nsec += t.tv_nsec;
	ret_val = ::pthread_mutex_timedlock(&_mutex, &timeout);
	return (!ret_val);
}
