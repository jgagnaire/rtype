#pragma once
#include "UnixSocketSet.hh"

class UnixTCPSocketSet : public UnixSocketSet {
public:
  UnixTCPSocketSet(unsigned short);
    virtual void absInitSocket(unsigned short);
  virtual IServerSocket<int> *absAcceptNewClient();
  virtual ~UnixTCPSocketSet() {}
};
