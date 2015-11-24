#pragma once
#include "WinSocketSet.hh"

class WinUDPSocketSet : public WinSocketSet {
protected:
	virtual void absInitSocket(unsigned short);
public:
        WinUDPSocketSet(unsigned short);
	virtual ~WinUDPSocketSet() {}
};
