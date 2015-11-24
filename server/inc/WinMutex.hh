#pragma once
#include <windows.h>
#include "IMutex.hh"

class WinMutex : public IMutex {
public:
	WinMutex();
	virtual ~WinMutex();
	virtual void	lock(void);
	virtual void	unlock(void);
	virtual bool	trylock(void);
private:
	HANDLE	_mutex;
};