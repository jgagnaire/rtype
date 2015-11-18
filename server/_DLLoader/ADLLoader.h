#ifndef ADLLOADER_H_
# define ADLLOADER_H_

# include <string>

template <typename PLUGIN>
class ADLLoader
{
public:
	ADLLoader();
	ADLLoader(std::string const &);
	virtual ~ADLLoader() {}
	virtual PLUGIN *getInstance();
	virtual void loadLib(std::string const &) = 0;

private:
	PLUGIN *(*fptr)();
	static PLUGIN *singleton;
};

#endif // !ADLLOADER_H_