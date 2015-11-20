#pragma once
#include <iostream>
#include "Enum.hh"

template <typename PUT, typename GET = PUT, typename FILE_HANDLE = int>
class IFifo {
  virtual ~IFifo() {}
  virtual void	closeFds() = 0;
  virtual void	closeUnixFifo() = 0;
  virtual void	communicationBetween(const std::string &s1,
				     const std::string &s2) = 0;
  virtual void	createUnixFifo(Enum::io_flag f) = 0;
  virtual bool	operator>>(const PUT &s) = 0;
  virtual bool	operator<<(GET &val) const = 0;
  virtual const std::string &getFirstName() const = 0;
  virtual const std::string &getSecondName() const = 0;
  virtual FILE_HANDLE	getFdIn() const = 0;
  virtual FILE_HANDLE	getFdOut() const = 0;
};
