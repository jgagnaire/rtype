#pragma once
#include "WinSocketSet.hh"

class WinTCPSocketSet : public WinSocketSet {
protected:
    virtual void absInitSocket(unsigned short);
public:
  WinTCPSocketSet(unsigned short);
  virtual ~WinTCPSocketSet() {}
  virtual IServerSocket<SOCKET> *absAcceptNewClient();
};
