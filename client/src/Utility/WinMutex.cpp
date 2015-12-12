#include "Utility/WinMutex.hh"
#include "ServerError.hh"

WinMutex::WinMutex() { }

WinMutex::~WinMutex()
{
	::CloseHandle(_mutex);
}

void	WinMutex::lock(void)
{
	_mutex = ::CreateMutex(0, TRUE, 0);
	if (!_mutex)
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
