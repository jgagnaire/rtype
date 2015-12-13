# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#   include "WinDLLoader.hh"
# else
#   include "UnixDLLoader.hh"
# endif

template <typename HANDLER>
ADLLoader<HANDLER> *ADLLoader<HANDLER>::adlloader = 0;

template <typename HANDLER>
ADLLoader<HANDLER>	*ADLLoader<HANDLER>::instance() {
  if (!ADLLoader::adlloader)
# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    return (ADLLoader<HANDLER>::adlloader = new WinDLLoader());
# else
  return (ADLLoader<HANDLER>::adlloader = new UnixDLLoader());
#endif
  return (adlloader);
};

# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
template class ADLLoader<HMODULE>;
# else
template class ADLLoader<void *>;
# endif
