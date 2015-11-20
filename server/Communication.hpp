#ifndef COMMUNICATION_HPP_
# define COMMUNICATION_HPP_

# include "Exceptions.hpp"
# include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <cstdio>
# include <errno.h>

template <typename PUT, typename GET = PUT>
class Communication {
private:
  int	_fdin;
  int	_fdout;
  std::string	_s1;
  std::string	_s2;

public:
  enum		io_flag {
    RD_TO_WR = 0,
    WR_TO_RD,
  };

  Communication() {
    _s1.clear();
    _s2.clear();
    _fdin = 0;
    _fdout = 0;
  }

  ~Communication() {
    closeCommunication();
  }

  Communication	&operator=(const Communication<GET, PUT> &com) {
    _fdin = com.getFdIn();
    _fdout = com.getFdOut();
    _s1 = com.getFirstName();
    _s2 = com.getSecondName();
    return (*this);
  }

  const std::string &getFirstName() const {
    return (_s1);
  }

  const std::string &getSecondName() const {
    return (_s2);
  }

  Communication(const Communication<GET, PUT> &com) {
    _fdin = com._fdin;
    _fdout = com._fdout;
    _s1 = com._s1;
    _s2 = com._s2;
  }


  void	closeFds() {
    if (_fdin != 0)
      close(_fdin);
    if (_fdout != 0)
      close(_fdout);
    _fdin = 0;
    _fdout = 0;
  }

  void	closeCommunication() {
    if (_fdin != 0)
      close(_fdin);
    if (_fdout != 0)
      close(_fdout);
    _fdin = 0;
    _fdout = 0;
    removeCommunication();
  }

  void	communicationBetween(const std::string &s1,
			     const std::string &s2) {
    _s1 = s1;
    _s2 = s2;
    if (!_s1.empty())
      if (mkfifo(_s1.c_str(), 0666) && EEXIST != errno)
	throw (FifoException("mkfifo failed"));
    if (!_s2.empty())
      if (mkfifo(_s2.c_str(), 0666) && EEXIST != errno)
	throw (FifoException("mkfifo failed"));
  }

  void	removeCommunication() {
    if (!_s1.empty())
      remove(_s1.c_str());
    if (!_s2.empty())
      remove(_s2.c_str());
    _s1.clear();
    _s2.clear();
  }

  void	createCommunication(io_flag f) {
    if (f == RD_TO_WR)
      {
	if (!_s1.empty())
	  if ((_fdin = open(_s1.c_str(), O_RDONLY)) == -1)
	    throw (FifoException("open failed"));
	if (!_s2.empty())
	  if ((_fdout = open(_s2.c_str(), O_WRONLY)) == -1)
	    throw (FifoException("open failed"));
      }
    else
      {
	if (!_s1.empty())
	  if ((_fdout = open(_s1.c_str(), O_WRONLY)) == -1)
	    throw (FifoException("open failed"));
	if (!_s2.empty())
	  if ((_fdin = open(_s2.c_str(), O_RDONLY)) == -1)
	    throw (FifoException("open failed"));
      }
  }

  int	getFdIn() const {
    return (_fdin);
  }

  int	getFdOut() const {
    return (_fdout);
  }

  bool	        operator>>(const PUT &s) {
    int		ret;

    ret = write(_fdout, &s, sizeof(s));
    return (!(ret <= 0));
  }


  bool	        operator<<(GET &val) const {
    int		ret;

    ret = read(_fdin, &val, sizeof(val));
    return (!(ret <= 0));
  }
};

template <>
class Communication<std::string, std::string> {
private:
  int	_fdin;
  int	_fdout;
  std::string	_s1;
  std::string	_s2;
public:
  enum		io_flag {
    RD_TO_WR = 0,
    WR_TO_RD,
  };

  Communication() {
    _s1.clear();
    _s2.clear();
    _fdin = 0;
    _fdout = 0;
  }

  ~Communication() {
    closeCommunication();
  }

  Communication	&operator=(const Communication<std::string, std::string> &com) {
    if (this == &com)
      return (*this);
    _fdin = com.getFdIn();
    _fdout = com.getFdOut();
    _s1 = com.getFirstName();
    _s2 = com.getSecondName();
    return (*this);
  }

  const std::string &getFirstName() const {
    return (_s1);
  }

  const std::string &getSecondName() const {
    return (_s2);
  }

  void	closeFds() {
    if (_fdin != 0)
      close(_fdin);
    if (_fdout != 0)
      close(_fdout);
    _fdin = 0;
    _fdout = 0;
  }

  void	closeCommunication() {
    if (_fdin != 0)
      close(_fdin);
    if (_fdout != 0)
      close(_fdout);
    _fdin = 0;
    _fdout = 0;
    removeCommunication();
  }

  void	communicationBetween(const std::string &s1,
			     const std::string &s2) {
    _s1 = s1;
    _s2 = s2;
    if (!_s1.empty())
      if (mkfifo(_s1.c_str(), 0666) && EEXIST != errno)
	throw (FifoException("mkfifo failed"));
    if (!_s2.empty())
      if (mkfifo(_s2.c_str(), 0666) && EEXIST != errno)
	throw (FifoException("mkfifo failed"));
  }

  void	removeCommunication() {
    if (!_s1.empty())
      remove(_s1.c_str());
    if (!_s2.empty())
      remove(_s2.c_str());
    _s1.clear();
    _s2.clear();
  }

  void	createCommunication(io_flag f) {
    if (f == RD_TO_WR)
      {
	if (!_s1.empty())
	  if ((_fdin = open(_s1.c_str(), O_RDONLY)) == -1)
	    throw (FifoException("open failed"));
	if (!_s2.empty())
	  if ((_fdout = open(_s2.c_str(), O_WRONLY)) == -1)
	    throw (FifoException("open failed"));
      }
    else
      {
	if (!_s1.empty())
	  if ((_fdout = open(_s1.c_str(), O_WRONLY)) == -1)
	    throw (FifoException("open failed"));
	if (!_s2.empty())
	  if ((_fdin = open(_s2.c_str(), O_RDONLY)) == -1)
	    throw (FifoException("open failed"));
      }
  }

  int	getFdIn() const {
    return (_fdin);
  }

  int	getFdOut() const {
    return (_fdout);
  }

  bool	        operator>>(const std::string &s) {
    int		ret;
    
    ret = write(_fdout, s.c_str(), s.length());
    return (!(ret <= 0));
  }

  bool	        operator<<(std::string &val) const {
    char		buff[255] = {0};
    int		ret;

    ret = read(_fdin, &buff, 255);
    val.assign(buff);
    return (!(ret <= 0));
  }
};

#endif
