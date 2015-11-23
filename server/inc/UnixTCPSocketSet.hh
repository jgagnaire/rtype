#pragma once
#include "UnixSocketSet.hh"

class UnixTCPSocketSet : public UnixSocketSet {
public:
  UnixTCPSocketSet();
  virtual IServerSocket<int> *absAcceptNewClient();
  virtual ~UnixTCPSocketSet() {}
};
