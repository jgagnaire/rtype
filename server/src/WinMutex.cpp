#include "WinMutex.h"
#include "Exceptions.h"

WinMutex::WinMutex() { }

WinMutex::~WinMutex()
{
	::CloseHandle(_mutex);
}

void	WinMutex::lock(void)
{
	if (!(_mutex = ::CreateMutex(0, TRUE, 0)))
		throw MutexException("Lock failed");
}

void	WinMutex::unlock(void)
{
	if (!ReleaseMutex(_mutex))
		throw MutexException("Unlock failed");
}

bool	WinMutex::trylock(void)
{
	return (!(::WaitForSingleObject(_mutex, 0) & (WAIT_ABANDONED | WAIT_FAILED)));
}