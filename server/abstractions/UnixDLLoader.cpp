#include "UnixDLLoader.hh"

UnixDLLoader::UnixDLLoader()
  : ADLLoader<void *>()
{}

UnixDLLoader::~UnixDLLoader() {}

bool	UnixDLLoader::openLib(std::string const &path, const std::string &key) {
  ADLLoader<void *>::DLInfo	info;
  void				*handle;

  if (key.empty() || !(handle = ::dlopen(path.c_str(), RTLD_NOW)))
    return (false);
  info.handle = handle;
  info.path = path;
  plugins[key] = info;
  return (true);
}

bool	UnixDLLoader::loadLib(const std::string &key, const std::string &sym) {
  auto	it = plugins.find(key);
  void	*tmp_sym;

  if (it == plugins.end())
    return (false);
  if (!(tmp_sym = ::dlsym(it->second.handle, sym.c_str())))
    return (false);
  it->second.fptr[sym] = reinterpret_cast<entry_point_fptr>(tmp_sym);
  return (true);
}

void	UnixDLLoader::closeLib(const std::string &key) {
  auto	it = plugins.find(key);

  if (it == plugins.end())
    return ;
  ::dlclose(it->second.handle);
  plugins.erase(it);
}

Entity	*UnixDLLoader::operator()(const std::string &key, const std::string &sym) {
  auto	it = plugins.find(key);

  if (it == plugins.end())
    return (0);
  auto it2 = it->second.fptr.find(sym); 
  if (it2 == it->second.fptr.end())
    return (0);
  return (it2->second());
}
