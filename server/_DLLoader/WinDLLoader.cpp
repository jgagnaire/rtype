#include "WinDLLoader.h"

template <typename PLUGIN>
WinDLLoader<PLUGIN>::WinDLLoader(std::string const &lib)
	: ADLLoader(), handle(0)
{
	this->loadLib(lib);
}

template <typename PLUGIN>
WinDLLoader<PLUGIN>::~WinDLLoader()
{
	if (this->handle)
		::FreeLibrary(this->handle);
}

template <typename PLUGIN>
void WinDLLoader<PLUGIN>::loadLib(std::string const &lib)
{
	if (!(this->handle = ::LoadLibrary(static cast<LPTSTR>(lib.c_str()), RTLD_NOW))
		|| !(this->funcptr = reinterpret_cast<T *(*)()>(::GetProcAddress(this->handle, "dl_entry_point"))))
	{
		LPVOID errbuf = 0;

		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_IGNORE_INSERTS, 0, ::GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			static_cast<LPTSTR>(&errbuf), 0, 0)
		throw DLError(errbuf);
	}
}