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

int WinServerSocket::absReadFromClient(char *&to_fill, unsigned size) const
{
	WSABUF wsabuf = { 0 };
	DWORD read_size = 0;
	DWORD flags = 0;

	wsabuf.buf = new char[size];
	wsabuf.len = size;
	if ((::WSARecv(this->sock, &wsabuf, 1, &read_size,
		&flags, NULL, NULL) == SOCKET_ERROR)
		&& (WSAGetLastError() != WSAEINTR))
	  {
	    delete wsabuf.buf;
	    return 0;
	  }
	to_fill = wsabuf.buf;
	return (read_size);
}

std::string	WinServerSocket::getIP() const {
  return (std::string(inet_ntoa(addr->sin_addr)));
}

bool WinServerSocket::absWriteOnClient(char *to_write, size_t size) const
{
	DWORD written = 0;
	WSABUF wsabuf = { 0 };
	char *tmp;

	wsabuf.len = size;
	wsabuf.buf = new char[size];
	tmp = wsabuf.buf;
	std::memcpy(wsabuf.buf, to_write, size);
	while (wsabuf.len > 0)
	{
		if ((::WSASend(this->sock, &wsabuf, 1, &written,
			NULL, NULL, NULL) == SOCKET_ERROR)
			&& (WSAGetLastError() != EINTR)) {
			return false;
		}
		wsabuf.len -= written;
		wsabuf.buf = &wsabuf.buf[written];
		written = 0;
	}
	delete [] tmp;
	return true;
}
