#include "WinMutex.hh"
#include "ServerError.hh"

WinMutex::WinMutex() { }

WinMutex::~WinMutex()
{
	::CloseHandle(_mutex);
}

inline
void	WinMutex::lock(void)
{
	_mutex = ::CreateMutex(0, TRUE, 0);
	if (!_mutex)
		throw MutexException("Lock failed");
}

inline
void	WinMutex::unlock(void)
{
	if (!ReleaseMutex(_mutex))
		throw MutexException("Unlock failed");
}

inline
bool	WinMutex::trylock(void)
{
	return (!(::WaitForSingleObject(_mutex, 0) & (WAIT_ABANDONED | WAIT_FAILED)));
}
