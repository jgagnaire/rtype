#include "UnixSocketSet.hh"

UnixSocketSet::UnixSocketSet(unsigned short port) {
  absInitSocket(port);
}

void UnixSocketSet::absInitSocket(unsigned short port)
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

IServerSocket<int> *UnixSocketSet::absAcceptNewClient() { return (0); }

