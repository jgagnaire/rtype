#pragma once
# include "IServerSocket.h"
# include "Enum.h"

template <typename T>
class IServerMonitor {
public:
	virtual void   addFd(IServerSocket<T> *, Enum::Flag flag) = 0;
	virtual void   closeFd(IServerSocket<T> *) = 0;
	virtual int   observerFds() = 0;
	virtual void   setObserver(IServerSocket<T> *, Enum::Flag flag) = 0;
	virtual void   setTimeval(unsigned sec, unsigned microsec = 0) = 0;
	virtual void   deleteFd(IServerSocket<T> *) = 0;
	virtual void   deleteTimeval() = 0;
	virtual bool   isObserved(IServerSocket<T> *, Enum::Flag flag) = 0;
	virtual		   ~IServerMonitor() {};
};
