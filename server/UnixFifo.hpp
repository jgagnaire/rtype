#pragma once
# include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <cstdio>
# include <errno.h>
# include "ServerError.hh"
# include "IFifo.hh"

template <typename PUT, typename GET = PUT>
class UnixFifo : public IFifo<PUT, GET, int> {
private:
  int		_fdin;
  int		_fdout;
  std::string	_s1;
  std::string	_s2;

  void	createUnixFifo(Enum::io_flag f) {
    if (f == Enum::RD_TO_WR)
      {
	if ((!_s1.empty()) && ((_fdin = ::open(_s1.c_str(), O_RDONLY)) == -1))
	    throw (FifoException("open failed"));
	if ((!_s2.empty()) && ((_fdout = ::open(_s2.c_str(), O_WRONLY)) == -1))
	    throw (FifoException("open failed"));
      }
    else
      {
	if ((!_s1.empty()) && ((_fdout = ::open(_s1.c_str(), O_WRONLY)) == -1))
	  throw (FifoException("open failed"));
	if ((!_s2.empty()) && ((_fdin = ::open(_s2.c_str(), O_RDONLY)) == -1))
	  throw (FifoException("open failed"));
      }
  }
public:
  UnixFifo() {
    _s1.clear();
    _s2.clear();
    _fdin = 0;
    _fdout = 0;
  }

  virtual ~UnixFifo() {
    closeFifo();
  }

  UnixFifo	&operator=(const UnixFifo<GET, PUT> &com) {
    _fdin = com.getFdIn();
    _fdout = com.getFdOut();
    _s1 = com.getFirstName();
    _s2 = com.getSecondName();
    return (*this);
  }

  const std::string &getFirstName() const { return (_s1); }

  const std::string &getSecondName() const { return (_s2); }

  UnixFifo(const UnixFifo<GET, PUT> &com) {
    _fdin = com._fdin;
    _fdout = com._fdout;
    _s1 = com._s1;
    _s2 = com._s2;
  }


  virtual void	closeFds() {
    if (_fdin != 0)
      ::close(_fdin);
    if (_fdout != 0)
      ::close(_fdout);
    _fdin = 0;
    _fdout = 0;
  }

  virtual void	closeFifo() {
    if (_fdin != 0)
      close(_fdin);
    if (_fdout != 0)
      close(_fdout);
    _fdin = 0;
    _fdout = 0;
    removeUnixFifo();
  }

  virtual void	communicationBetween(const std::string &s1,
				     const std::string &s2,
				     Enum::io_flag f) {
    _s1 = s1;
    _s2 = s2;
    if (!_s1.empty())
      if (::mkfifo(_s1.c_str(), 0666) && EEXIST != errno)
	throw (FifoException("mkfifo failed"));
    if (!_s2.empty())
      if (::mkfifo(_s2.c_str(), 0666) && EEXIST != errno)
	throw (FifoException("mkfifo failed"));
    createUnixFifo(f);
  }

  void	removeUnixFifo() {
    if (!_s1.empty())
      remove(_s1.c_str());
    if (!_s2.empty())
      remove(_s2.c_str());
    _s1.clear();
    _s2.clear();
  }

  virtual int	getFdIn() const { return (_fdin); }

  virtual int	getFdOut() const { return (_fdout); }

  virtual bool	        operator>>(const PUT &s) {
    int		ret;

    ret = ::write(_fdout, &s, sizeof(s));
    return (!(ret <= 0));
  }


  virtual bool	        operator<<(GET &val) const {
    int		ret;

    ret = ::read(_fdin, &val, sizeof(val));
    return (!(ret <= 0));
  }
};

template <>
class UnixFifo<std::string, std::string> :
  public IFifo<std::string, std::string, int> {
private:
  int		_fdin;
  int		_fdout;
  std::string	_s1;
  std::string	_s2;

  void	createUnixFifo(Enum::io_flag f) {
    if (f == Enum::RD_TO_WR)
      {
	if ((!_s1.empty()) && ((_fdin = ::open(_s1.c_str(), O_RDONLY)) == -1))
	    throw (FifoException("open failed"));
	if ((!_s2.empty()) && ((_fdout = ::open(_s2.c_str(), O_WRONLY)) == -1))
	    throw (FifoException("open failed"));
      }
    else
      {
	if ((!_s1.empty()) && ((_fdout = ::open(_s1.c_str(), O_WRONLY)) == -1))
	  throw (FifoException("open failed"));
	if ((!_s2.empty()) && ((_fdin = ::open(_s2.c_str(), O_RDONLY)) == -1))
	  throw (FifoException("open failed"));
      }
  }
public:

  UnixFifo() {
    _s1.clear();
    _s2.clear();
    _fdin = 0;
    _fdout = 0;
  }

  ~UnixFifo() {
    closeFifo();
  }

  UnixFifo	&operator=(const UnixFifo<std::string, std::string> &com) {
    if (this == &com)
      return (*this);
    _fdin = com.getFdIn();
    _fdout = com.getFdOut();
    _s1 = com.getFirstName();
    _s2 = com.getSecondName();
    return (*this);
  }

  virtual const std::string &getFirstName() const { return (_s1); }

  virtual const std::string &getSecondName() const { return (_s2); }

  virtual void	closeFds() {
    if (_fdin != 0)
      ::close(_fdin);
    if (_fdout != 0)
      ::close(_fdout);
    _fdin = 0;
    _fdout = 0;
  }

  virtual void	closeFifo() {
    closeFds();
    removeUnixFifo();
  }

  virtual void	communicationBetween(const std::string &s1,
				     const std::string &s2, Enum::io_flag f) {
    _s1 = s1;
    _s2 = s2;
    if (!_s1.empty())
      if (mkfifo(_s1.c_str(), 0666) && EEXIST != errno)
	throw (FifoException("mkfifo failed"));
    if (!_s2.empty())
      if (mkfifo(_s2.c_str(), 0666) && EEXIST != errno)
	throw (FifoException("mkfifo failed"));
    createUnixFifo(f);
  }

  void	removeUnixFifo() {
    if (!_s1.empty())
      remove(_s1.c_str());
    if (!_s2.empty())
      remove(_s2.c_str());
    _s1.clear();
    _s2.clear();
  }

  virtual int	getFdIn() const { return (_fdin); }

  virtual int	getFdOut() const { return (_fdout); }

  virtual bool	operator>>(const std::string &s) {
    int		ret;
    
    ret = write(_fdout, s.c_str(), s.length());
    return (!(ret <= 0));
  }

  virtual bool	operator<<(std::string &val) const {
    char	buff[255] = {0};
    int		ret;

    ret = read(_fdin, &buff, 255);
    val.assign(buff);
    return (!(ret <= 0));
  }
};
