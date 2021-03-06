#pragma once
#include <pthread.h>
#include "IMutex.hh"

class UnixMutex : public IMutex
{
public:
	UnixMutex();
	virtual ~UnixMutex();
	virtual void	lock(void);
	virtual void	unlock(void);
	virtual bool	trylock(void);
	bool			timedlock(const struct timespec &t);
private:
	pthread_mutex_t	_mutex;
};
