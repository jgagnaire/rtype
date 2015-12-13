#include "UnixSocketSet.hh"

UnixSocketSet::UnixSocketSet(unsigned short port) {
  absInitSocket(port);
}

inline
IServerSocket<int> *UnixSocketSet::absAcceptNewClient() { return (0); }

