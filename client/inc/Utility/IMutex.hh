#pragma once

class IMutex
{
public:
	virtual ~IMutex() {}
	virtual void lock(void) = 0;
	virtual void unlock(void) = 0;
	virtual bool trylock(void) = 0;
};
