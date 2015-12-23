#include "UnixSocketSet.hh"

UnixSocketSet::UnixSocketSet(unsigned short port) {
  absInitSocket(port);
}

IServerSocket<int> *UnixSocketSet::absAcceptNewClient() { return (0); }

