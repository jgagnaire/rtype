#pragma once

#include <winsock2.h>
#include <windows.h>
#include "WinServerSocket.hh"
#include "ISocketSet.hh"
#include "ServerError.hh"

class WinSocketSet : public WinServerSocket, public ISocketSet<SOCKET>  {
protected:
  virtual void absInitSocket(unsigned short) {};
public:
  WinSocketSet(unsigned short);
  virtual ~WinSocketSet() {}
  virtual IServerSocket<SOCKET> *absAcceptNewClient();
};
