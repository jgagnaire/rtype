#ifndef SERVERERROR_H_
# define SERVERERROR_H_

# include <exception>
# include <stdexcept>
# include <sstream>
# include <string>

enum eErrorType
  {
	ESOCKCREATE = 0,
	ESOCKACCEPT,
	ESOCKREAD,
	ESOCKWRITE,
	ESOCKDYNAFAIL,
	ESOCKCREATEEVENT,
	ESOCKEVENTSELECT,
	EMONITORFAILED,
  };

class JSONException : public std::runtime_error
{
public:
  JSONException(const char *msg) :
    runtime_error(msg) {}
  JSONException(const std::string &msg) :
    runtime_error(msg) {}
  virtual ~JSONException() throw() {}
};

#endif // !SERVERERROR_H_
