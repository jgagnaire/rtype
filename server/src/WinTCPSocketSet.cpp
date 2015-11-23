#include "WinTCPSocketSet.hh"

WinTCPSocketSet::WinTCPSocketSet(unsigned short port)
  : WinSocketSet(port) {}

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
