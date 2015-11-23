#ifndef ADLLOADER_H_
# define ADLLOADER_H_

# include <string>
# include "Entity.hh"

typedef Entity (*entry_point_fptr)(void);

template <typename PLUGIN>
class ADLLoader
{
public:
	ADLLoader(std::string const &);
	virtual ~ADLLoader() {}
	virtual void loadLib(std::string const &) = 0;

private:
	ADLLoader() {}
	std::unordered_map<std::size_t, entry_point_fptr> plugins_map;
};

#endif // !ADLLOADER_H_