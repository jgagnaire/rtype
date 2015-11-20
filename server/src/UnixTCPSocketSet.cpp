#include "UnixTCPSocketSet.hh"

UnixTCPSocketSet::UnixTCPSocketSet(unsigned short port)
  : UnixSocketSet(port) {}

IServerSocket<int> *UnixTCPSocketSet::absAcceptNewClient()
{
  int clfd = 0;
  unsigned socksize = sizeof(sockaddr_in);
  sockaddr_in cl_addr = { 0, 0, 0, 0 };

  try {
    if ((clfd = ::accept(getSocket(),
			 reinterpret_cast<sockaddr *>(&cl_addr),
			 &socksize)) == -1)
      throw ESockAccept();
  }
  catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return 0;
  }
  return new UnixServerSocket(clfd, &cl_addr);
}
