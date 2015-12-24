#include "WinServerSocket.hh"

WinServerSocket::WinServerSocket()
	: addr(0)
{
	if (::WSAStartup(MAKEWORD(2, 2), &this->wsaData))
		throw ESockCreate();
	if (LOBYTE(this->wsaData.wVersion) != 2
		|| HIBYTE(this->wsaData.wVersion) != 2)
		{
			::WSACleanup();
			throw ESockCreate();
		}
}

WinServerSocket::WinServerSocket(SOCKET sck, sockaddr_in *addr)
 : sock(sck)
{
	this->addr = new sockaddr_in;
	*this->addr = *addr;
}

WinServerSocket::~WinServerSocket()
{
	::closesocket(this->sock);
}

int WinServerSocket::absReadFromClient(char *&to_fill, unsigned size, std::string *const ip) const
{
	WSABUF wsabuf = { 0 };
	DWORD read_size = 0;
	DWORD flags = 0;
	sockaddr_in from;
	int from_size = sizeof(from);

	wsabuf.buf = new char[size];
	wsabuf.len = size;
	if ((::WSARecvFrom(this->sock, &wsabuf, 1, &read_size,
		&flags, reinterpret_cast<struct sockaddr *>(&from), &from_size,
					   0, 0) == SOCKET_ERROR)
		&& (WSAGetLastError() != WSAEINTR))
	  {
	    delete wsabuf.buf;
	    return 0;
	  }
	to_fill = wsabuf.buf;
	if (ip != NULL)
	{
		char buf[22] = { 0 };
		::inet_ntop(AF_INET, &from.sin_addr, &buf[0], INET_ADDRSTRLEN);
		*ip = std::string(&buf[0]);
	}
	return (read_size);
}

std::string	WinServerSocket::getIP() const {
	char buf[22] = { 0 };
	::inet_ntop(AF_INET, &addr->sin_addr, &buf[0], INET_ADDRSTRLEN);
	return (std::string(&buf[0]));
}

bool WinServerSocket::absWriteOnClient(char *to_write, uint64_t size,
				       const std::string &ip,
				       const std::string &port) const
{
	DWORD written = 0;
	WSABUF wsabuf = { 0 };
	char *tmp;
	sockaddr_in *to = 0;

	if (!(ip.empty() || port.empty())) {
		to = new sockaddr_in;
		to->sin_family = AF_INET;
		to->sin_port = ::htons(static_cast<uint16_t>(std::stoi(port)));
		::inet_pton(AF_INET, ip.c_str(), reinterpret_cast<in_addr *>(&to->sin_addr.s_addr));
	}
	wsabuf.len = size;
	wsabuf.buf = new char[size];
	tmp = wsabuf.buf;
	std::memcpy(wsabuf.buf, to_write, size);
	while (wsabuf.len > 0)
	{
		if ((::WSASendTo(this->sock, &wsabuf, 1, &written,
			0, reinterpret_cast<struct sockaddr *>(to), sizeof(*to), 0, 0) == SOCKET_ERROR)
			&& (WSAGetLastError() != WSAEINTR)) {
			return false;
		}
		wsabuf.len -= written;
		wsabuf.buf = &wsabuf.buf[written];
		written = 0;
	}
	delete [] tmp;
	delete to;
	return true;
}
