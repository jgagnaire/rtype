#pragma once
#include <pthread.h>
#include "IMutex.h"

class UnixMutex : public IMutex
{
public:
	UnixMutex();
	virtual ~CUMutex();
	virtual void	lock(void);
	virtual void	unlock(void);
	virtual bool	trylock(void);
	bool			timedlock(const struct timespec &t);
private:
	pthread_mutex_t	_mutex;
};