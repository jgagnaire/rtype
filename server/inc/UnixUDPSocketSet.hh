#pragma once
#include "UnixSocketSet.hh"

class UnixUDPSocketSet : public UnixSocketSet {
protected:
  virtual void absInitSocket(unsigned short);
public:
  UnixUDPSocketSet(unsigned short);
  virtual ~UnixUDPSocketSet() {}
};
