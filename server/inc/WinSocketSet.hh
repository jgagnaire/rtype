#pragma once

#include <winsock2.h>
#include <windows.h>
#include <WinServerSocket.h>
#include <ISocketSet.h>
#include "ServerError.h"

class WinSocketSet : public WinServerSocket, public ISocketSet<SOCKET>  {
protected:
	virtual void absInitSocket(unsigned short);
public:
	WinSocketSet(unsigned short);
	virtual ~WinSocketSet() {}
	virtual IServerSocket<SOCKET> *absAcceptNewClient();
};