#ifndef SERVERERROR_H_
# define SERVERERROR_H_

# include <exception>
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

class ServerError : public std::exception
{
public:
  ServerError() {}
  ServerError(eErrorType);
  ServerError(ServerError const &) {}
  virtual ~ServerError() throw();
  virtual const char *what() const throw();
  std::stringstream const &getS() const;
  void buildString(eErrorType);
private:
  std::stringstream s;
};

class ESockCreate : public ServerError
{
public:
  ESockCreate(eErrorType type = ESOCKCREATE)
  {
    this->buildString(type);
  }
  virtual ~ESockCreate() throw() {}
};

class ESockAccept : public ServerError
{
public:
  ESockAccept(eErrorType type = ESOCKACCEPT)
  {
    this->buildString(type);
  }
  virtual ~ESockAccept() throw() {}
};

class ESockRead : public ServerError
{
public:
  ESockRead(eErrorType type = ESOCKREAD)
  {
    this->buildString(type);
  }
  virtual ~ESockRead() throw() {}
};

class ESockWrite : public ServerError
{
public:
  ESockWrite(eErrorType type = ESOCKWRITE)
  {
    this->buildString(type);
  }
  virtual ~ESockWrite() throw() {}
};

class ESockDynaFail : public ServerError
{
public:
	ESockDynaFail(eErrorType type = ESOCKDYNAFAIL)
	{
		this->buildString(type);
	}
	virtual ~ESockDynaFail() throw() {}
};

class ESockCreateEvent : public ServerError
{
public:
	ESockCreateEvent(eErrorType type = ESOCKCREATEEVENT)
	{
		this->buildString(type);
	}
	virtual ~ESockCreateEvent() throw() {}
};

class ESockEventSelect : public ServerError
{
public:
	ESockEventSelect(eErrorType type = ESOCKEVENTSELECT)
	{
		this->buildString(type);
	}
	virtual ~ESockEventSelect() throw() {}
};

class EMonitorFailed : public ServerError
{
public:
	EMonitorFailed(eErrorType type = EMONITORFAILED)
	{
		this->buildString(type);
	}
	virtual ~EMonitorFailed() throw() {}
};

#endif // !SERVERERROR_H_
