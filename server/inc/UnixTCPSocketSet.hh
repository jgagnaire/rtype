#pragma once
#include "UnixSocketSet.hh"

class UnixTCPSocketSet : public UnixSocketSet {
protected:
    virtual void absInitSocket(unsigned short);
public:
  UnixTCPSocketSet(unsigned short);
  virtual IServerSocket<int> *absAcceptNewClient();
  virtual ~UnixTCPSocketSet() {}
};
