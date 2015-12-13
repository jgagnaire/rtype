#include "WinUDPSocketSet.hh"

WinUDPSocketSet::WinUDPSocketSet(unsigned short port)
  : WinSocketSet(port) { this->absInitSocket(port); }

void	WinUDPSocketSet::absInitSocket(unsigned short port) {
  SOCKET sock = 1;
  sockaddr_in srv_addr = { 0, 0, 0, 0 };

  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = ::htons(port);
  srv_addr.sin_addr.s_addr = INADDR_ANY;
  if ((sock = ::WSASocketW(AF_INET, SOCK_DGRAM,
			  IPPROTO_UDP,
			  NULL, 0, 0)) == INVALID_SOCKET
      || bind(sock, reinterpret_cast<const sockaddr *>(&srv_addr),
	      sizeof(struct sockaddr_in)) == SOCKET_ERROR)
    {
      ::WSACleanup();
      throw ESockCreate();
    }
  setSocket(sock);
}
