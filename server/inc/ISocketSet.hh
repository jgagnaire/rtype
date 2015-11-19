#pragma once
#include <IServerSocket.h>

template <typename T>
class ISocketSet {
protected:
	virtual void absInitSocket(unsigned short) = 0;
public:
	virtual ~ISocketSet() {}
	virtual IServerSocket<T> *absAcceptNewClient() = 0;
};