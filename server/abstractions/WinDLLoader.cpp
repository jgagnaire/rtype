#include "WinDLLoader.hh"

WinDLLoader::WinDLLoader()
  : ADLLoader<HMODULE>()
{}

WinDLLoader::~WinDLLoader() {}

bool	WinDLLoader::openLib(std::string const &path, const std::string &key) {
  ADLLoader<HMODULE>::DLInfo	info;
  HMODULE			handle;

  if (key.empty() || !(handle = ::LoadLibrary(path.c_str(), RTLD_NOW)))
    return (false);
  info.handle = handle;
  info.path = path;
  plugins[key] = info;
  return (true);
}

bool	WinDLLoader::loadLib(const std::string &key, const std::string &sym) {
  auto	it = plugins.find(key);
  FARPROC	tmp_sym;

  if (it == plugins.end())
    return (false);
  if (!(tmp_sym = getProcAddress(it->second.handle, sym.c_str())))
    return (false);
  it->second.fptr[sym] = reinterpret_cast<entry_point_fptr>(tmp_sym);
  return (true);
}

void	WinDLLoader::closeLib(const std::string &key) {
  auto	it = plugins.find(key);

  if (it == plugins.end())
    return ;
  ::dlclose(it->second.handle);
  plugins.erase(it);
}

Entity	*WinDLLoader::operator()(const std::string &key, const std::string &sym) {
  auto	it = plugins.find(key);

  if (it == plugins.end())
    return (0);
  auto it2 = it->second.fptr.find(sym); 
  if (it2 == it->second.fptr.end())
    return (0);
  return (it2->second());
}
