#pragma once
#include "IServerSocket.h"

template <typename T>
class IUserManager<T> {
public:
  virtual ~IUserManager() {}
  virtual bool	emptyData() const = 0;
  virtual bool	sendStructEmpty() const = 0;
  virtual bool	writeOnMe() = 0;
  virtual IServerSocket<T>	*getServerSocket() = 0;
  virtual bool	IsFilled() const = 0;
  virtual void	clearData() = 0;
};
