#include "UnixTCPSocketSet.hh"

UnixTCPSocketSet::UnixTCPSocketSet(unsigned short port)
  : UnixSocketSet(port) {
  this->absInitSocket(port);
}

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


void UnixTCPSocketSet::absInitSocket(unsigned short port)
{
  int sockfd;
  protoent *p;
  sockaddr_in srv_addr = { 0, 0, 0, 0 };
  int	tmp = 1;

  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = ::htons(port);
  srv_addr.sin_addr.s_addr = INADDR_ANY;
  if (!(p = getprotobyname("TCP"))
      || (sockfd = ::socket(AF_INET, SOCK_STREAM, p->p_proto)) == -1
      || ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) == -1
      || ::bind(sockfd, reinterpret_cast<const sockaddr *>(&srv_addr),
                sizeof(sockaddr_in)) == -1
      || ::listen(sockfd, SOMAXCONN) == -1)
    throw ESockCreate();
  setSocket(sockfd);
}
