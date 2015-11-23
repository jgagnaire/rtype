#ifndef UNIXDLLOADER_H_
# define UNIXDLLOADER_H_

# include <iostream>
# include <dlfcn.h>
# include <string>
# include "ADLLoader.h"

template <typename PLUGIN>
class UnixDLLoader : public ADLLoader<PLUGIN>
{
public:
	UnixDLLoader()
		: ADLLoader(), handle(0) {}
	virtual ~UnixDLLoader();
	virtual void loadLib(std::string const &);

private:
	void *handle;
};

#endif // !UNIXDLLOADER_H_