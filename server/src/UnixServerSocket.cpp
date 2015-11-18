#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "UnixServerSocket.h"

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

std::string	UnixServerSocket::getIP() const {
  return (std::string(inet_ntoa(addr->sin_addr)));
}

int UnixServerSocket::absReadFromClient(char *&to_fill, unsigned size) const
{
  char *buf = new char [size];
  int  ret;

  if ((ret = ::read(this->sockfd, buf, size)) == -1
      and errno != EINTR)
    return false;
  if (ret != 0)
    to_fill = buf;
  else {
    delete [] buf;
    to_fill = NULL;
  }
  return (ret);
}

bool UnixServerSocket::absWriteOnClient(char *tmp, size_t size) const
{
  size_t val = 0;
  int	ret;

  while (size != val)
    {
      if ((ret = ::write(this->sockfd, tmp,
			 size - val)) == -1
	  and errno != EINTR)
	return false;
      val += static_cast<size_t>(ret);
    }
  return true;
}
