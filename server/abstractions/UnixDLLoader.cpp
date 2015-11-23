#include "UnixDLLoader.h"

template <typename T>
UnixDLLoader<T>::UnixDLLoader(std::string const &lib)
  : ADLLoader(), handle(0)
{
	this->loadLib(lib);
}

template <typename T>
UnixDLLoader<T>::~UnixDLLoader()
{
  if (this->handle)
    ::dlclose(this->handle);
}

template <typename T>
void UnixDLLoader<T>::loadLib(std::string const &lib)
{
  if (!(this->handle = ::dlopen(lib.c_str(), RTLD_NOW))
      || !(this->funcptr = reinterpret_cast<T *(*)()>(::dlsym(this->handle, "dl_entry_point"))))
    throw DLError(::dlerror());
}
