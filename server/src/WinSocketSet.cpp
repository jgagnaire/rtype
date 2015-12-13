#include "WinSocketSet.hh"

WinSocketSet::WinSocketSet(unsigned short port) {
	absInitSocket(port);
}

inline
IServerSocket<SOCKET> *WinSocketSet::absAcceptNewClient() { return (0); }
