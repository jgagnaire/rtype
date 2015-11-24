#pragma once
# include "WinMutex.h"
# include "UnixMutex.h"

class Mutex
{
public:
	static IMutex	*create()
	{
# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
		return (new WinMutex());
# else
		return (new UnixMutex());
# endif
	}
};