#pragma once
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include "ISocketSet.hh"
#include "UnixServerSocket.hh"

class UnixSocketSet : public ISocketSet<int>, public UnixServerSocket {
protected:
    virtual void absInitSocket(unsigned short) {};
public:
  UnixSocketSet(unsigned short);
  virtual ~UnixSocketSet() {}
  virtual IServerSocket<int> *absAcceptNewClient();
};
