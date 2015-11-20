#pragma once
#include "WinSocketSet.hh"

class WinTCPSocketSet : public WinSocketSet {
public:
  WinTCPSocketSet(unsigned short);
  virtual ~WinTCPSocketSet() {}
  virtual IServerSocket<SOCKET> *absAcceptNewClient();
};
