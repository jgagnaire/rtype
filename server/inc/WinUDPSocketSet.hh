#pragma once
#include "WinSocketSet.h"

class WinUDPSocketSet : public WinSocketSet {
protected:
	virtual void absInitSocket(unsigned short);
public:
        WinUDPSocketSet(unsigned short);
	virtual ~WinUDPSocketSet() {}
}
