#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "UnixServerSocket.hh"

UnixServerSocket::UnixServerSocket()
  : sockfd(-1), addr(0)
{
}

UnixServerSocket::UnixServerSocket(int fd, sockaddr_in *addr)
 : sockfd(fd)
{
  this->addr = new sockaddr_in;
  *this->addr = *addr;
}

UnixServerSocket::~UnixServerSocket()
{
  if (this->sockfd != -1)
    ::close(this->sockfd);
  delete this->addr;
}

inline
std::string	UnixServerSocket::getIP() const {
  return (std::string(inet_ntoa(addr->sin_addr)));
}

int UnixServerSocket::absReadFromClient(char *&to_fill, unsigned size, std::string * const ip) const
{
  char *buf = new char [size];
  int  ret;
  sockaddr_in from;
  socklen_t   tmp = sizeof(from);

  if ((ret = ::recvfrom(this->sockfd, buf, size,
                        0, reinterpret_cast<struct sockaddr *>(&from), &tmp)) == -1
      and errno != EINTR)
    return false;
  if (ret != 0)
    to_fill = buf;
  else {
    delete [] buf;
    to_fill = NULL;
  }
  if (ip != NULL)
    *ip = std::string(inet_ntoa(from.sin_addr));
  return (ret);
}

bool UnixServerSocket::absWriteOnClient(char *tmp, size_t size,
                                        const std::string &ip, const std::string &port) const
{
  size_t val = 0;
  int	ret;
  sockaddr_in *to = 0;

  if (!(ip.empty() && port.empty())) {
    to = new sockaddr_in;
    to->sin_family = AF_INET;
    to->sin_port = ::htons(static_cast<uint16_t>(std::stoi(port)));
    ::inet_aton(ip.c_str(), reinterpret_cast<in_addr *>(&to->sin_addr.s_addr));
  }
  while (size > val)
    {
      if ((ret = ::sendto(this->sockfd, tmp,
			 size - val, 0, reinterpret_cast<struct sockaddr *>(to), sizeof(to))) == -1
	  and errno != EINTR)
	return false;
      val += static_cast<size_t>(ret);
    }
  return true;
}
