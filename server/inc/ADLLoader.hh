#ifndef ADLLOADER_H_
# define ADLLOADER_H_

# include <cstddef>
# include <string>
# include <array>
# include <unordered_map>
# include "Entity.hh"

template <typename HANDLER>
class ADLLoader
{
public:
  typedef Entity *(*entry_point_fptr)(void);

  struct DLInfo {
    HANDLER						handle;
    std::unordered_map<std::string, entry_point_fptr>	fptr;
    std::string						path;
  };

  virtual bool		openLib(std::string const &path, const std::string &key) = 0;
  virtual bool		loadLib(const std::string &key, const std::string &sym) = 0;
  virtual void		closeLib(const std::string &key) = 0;
  virtual Entity	*operator()(const std::string &key, const std::string &sym) = 0;
  static ADLLoader	*instance();

protected:
  ADLLoader() {}
  ADLLoader(const ADLLoader &) {}
  ADLLoader &operator=(const ADLLoader &) { return (*this); }
  virtual ~ADLLoader() {}

  static ADLLoader			  *adlloader;
  std::unordered_map<std::string, DLInfo> plugins;
};

#endif // !ADLLOADER_H_
