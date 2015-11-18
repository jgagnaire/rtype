#ifndef WINDLLOADER_H_
# define WINDLLOADER_H_

# include <windows.h> 
# include <string>
# include "ADLLoader.h"

template <typename PLUGIN>
class WinDLLoader : public ADLLoader<PLUGIN>
{
public:
	WinDLLoader() {}
	WinDLLoader(std::string const &);
	virtual ~WinDLLoader();
	virtual PLUGIN *getInstance();
	virtual void loadLib(std::string const &);

private:
	HINSTANCE handle;
};

#endif // !WINDLLOADER_H_