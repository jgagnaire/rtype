#include "WinTCPSocketSet.hh"

WinTCPSocketSet::WinTCPSocketSet(unsigned short port)
  : WinSocketSet(port) { this->absInitSocket(port); }

IServerSocket<SOCKET> *WinTCPSocketSet::absAcceptNewClient() {
  SOCKET clsock;
  int socksize = sizeof(sockaddr_in);
  sockaddr_in cl_addr = { 0, 0, 0, 0 };

  try {
    if ((clsock = ::WSAAccept(getSocket(),
			      reinterpret_cast<SOCKADDR *>(&cl_addr),
			      &socksize, NULL, NULL)) == INVALID_SOCKET)
      throw ESockAccept();
  }
  catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return 0;
  }
  return new WinServerSocket(clsock, &cl_addr);
}

void WinTCPSocketSet::absInitSocket(unsigned short port)
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