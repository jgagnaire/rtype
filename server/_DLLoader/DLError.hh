#ifndef DLERROR_H_
# define DLERROR_H_

# include <stdexcept>
# include <sstream>

class DLError : public std::runtime_error
{
public:
	DLError(char const *e)
		: runtime_error(e) {}
	virtual ~DLError() throw() {}
};

#endif /* !DLERROR_H_ */