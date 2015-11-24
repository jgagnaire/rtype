#ifndef WINSERVERSOCKET_H_
# define WINSERVERSOCKET_H_

# include <winsock2.h>
# include <iostream>
# include <cstring>
# include <string>
# include "IServerSocket.hh"
# include "ServerError.hh"

# pragma comment(lib, "ws2_32.lib")

class WinServerSocket : public IServerSocket<SOCKET>
{
public:
	WinServerSocket();
	WinServerSocket(SOCKET, sockaddr_in *);
	virtual ~WinServerSocket();
	virtual int absReadFromClient(char *&, unsigned, std::string *) const;
	virtual bool absWriteOnClient(char *, size_t, const std::string &, const std::string &) const;
	virtual std::string   getIP() const;
private:
	WSADATA wsaData;
	SOCKET sock;
	sockaddr_in *addr;
public:
	virtual inline SOCKET			getSocket() const { return(this->sock); }
protected:
	virtual inline void				setSocket(SOCKET socket) { this->sock = socket; }
};

#endif // !WINSERVERSOCKET_H_
