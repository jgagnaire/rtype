#pragma once
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <list>
#include <vector>
#include "WinServerSocket.hh"
#include "IServerMonitor.hh"
#include "Enum.hh"

class WinServerMonitor : public IServerMonitor<SOCKET> {
public:
	virtual ~WinServerMonitor();
	WinServerMonitor();
	virtual void   addFd(IServerSocket<SOCKET> *, Enum::Flag socket_flag);
	virtual void   closeFd(IServerSocket<SOCKET> *);
	virtual void   deleteFd(IServerSocket<SOCKET> *);
	virtual int	   observerFds();
	virtual void   setObserver(IServerSocket<SOCKET> *, Enum::Flag socket_flag);
	virtual bool   isObserved(IServerSocket<SOCKET> *, Enum::Flag socket_flag);
	virtual void   setTimeval(unsigned sec, unsigned micro = 0);
	virtual void   deleteTimeval();
private:
	struct all_flags {
		Enum::Flag		monitor_flag;
		long			win_flag;
	};
	std::vector<all_flags>	flags;
	DWORD	 _max_fd;
	WSAEVENT _events[WSA_MAXIMUM_WAIT_EVENTS];
	SOCKET   _sockets[WSA_MAXIMUM_WAIT_EVENTS];
	DWORD	 _timeout;
	DWORD	 _ret_val;
	DWORD	 _loop_event;
};
