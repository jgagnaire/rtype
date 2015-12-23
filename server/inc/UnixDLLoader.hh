#ifndef UNIXDLLOADER_H_
# define UNIXDLLOADER_H_

# include <dlfcn.h>
# include <string>
# include "ADLLoader.hh"

class UnixDLLoader : public ADLLoader<void *>
{
public:
  UnixDLLoader();
  virtual ~UnixDLLoader();

  virtual bool		openLib(std::string const &path, const std::string &key);
  virtual bool		loadLib(const std::string &key, const std::string &sym);
  virtual void		closeLib(const std::string &key);
  virtual Entity	*operator()(const std::string &key, const std::string &sym);
};

#endif // !UNIXDLLOADER_H_
