#include "WinSocketSet.hh"

WinSocketSet::WinSocketSet(unsigned short port) {
	absInitSocket(port);
}

IServerSocket<SOCKET> *WinSocketSet::absAcceptNewClient() { return (0); }
