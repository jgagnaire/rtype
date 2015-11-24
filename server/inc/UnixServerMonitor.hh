#pragma once
# include <list>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>
# include <signal.h>
# include "UnixServerSocket.hh"
# include "IServerMonitor.hh"
# include "Enum.hh"

class UnixServerMonitor : public IServerMonitor<int> {
public:
	UnixServerMonitor();
	virtual ~UnixServerMonitor();
	virtual void   addFd(IServerSocket<int> *, Enum::Flag socket_flag);
	virtual void   closeFd(IServerSocket<int> *);
	virtual void   deleteFd(IServerSocket<int> *);
	virtual int		observerFds();
	virtual void   setObserver(IServerSocket<int> *, Enum::Flag socket_flag);
	virtual bool   isObserved(IServerSocket<int> *, Enum::Flag socket_flag);
	virtual void   setTimeval(unsigned sec, unsigned micro = 0);
	virtual void   deleteTimeval();
private:
	void		   setObserver(int fd, Enum::Flag socket_flag);
	struct select_info {
		int  fd;
		Enum::Flag flag;
	};
	fd_set   readfds;
	fd_set   writefds;
	int    fd_nbr;
	struct timeval  *timeout;
	std::list<select_info *> fds;
};
