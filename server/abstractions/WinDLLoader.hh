#ifndef WINDLLOADER_H_
# define WINDLLOADER_H_

# include <windows.h> 
# include <string>
# include "ADLLoader.hh"

class WinDLLoader : public ADLLoader<void *>
{
public:
  WinDLLoader();
  virtual ~WinDLLoader();

  virtual bool		openLib(std::string const &path, const std::string &key);
  virtual bool		loadLib(const std::string &key, const std::string &sym);
  virtual void		closeLib(const std::string &key);
  virtual Entity	*operator()(const std::string &key, const std::string &sym);
};

#endif // !WINDLLOADER_H_
