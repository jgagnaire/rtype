#include "WinSocketSet.h"

WinSocketSet::WinSocketSet(unsigned short port) {
	absInitSocket(port);
}

void WinSocketSet::absInitSocket(unsigned short port)
{
  SOCKET sock = 1;
  sockaddr_in srv_addr = { 0, 0, 0, 0 };

  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = ::htons(port);
  srv_addr.sin_addr.s_addr = INADDR_ANY;
  if ((sock = ::WSASocket(AF_INET, SOCK_STREAM,
			  IPPROTO_TCP,
			  NULL, 0, 0)) == INVALID_SOCKET
      || bind(sock, reinterpret_cast<const sockaddr *>(&srv_addr),
	      sizeof(struct sockaddr_in)) == SOCKET_ERROR
      || listen(sock, SOMAXCONN) == SOCKET_ERROR)
    {
      ::WSACleanup();
      throw ESockCreate();
    }
  setSocket(sock);
}

IServerSocket<SOCKET> *WinSocketSet::absAcceptNewClient() { return (0); }
