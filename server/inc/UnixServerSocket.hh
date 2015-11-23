#ifndef UNIXSERVERSOCKET_H_
# define UNIXSERVERSOCKET_H_

# include <netinet/in.h>
# include <iostream>
# include <unistd.h>
# include <errno.h>
# include <netdb.h>
# include "IServerSocket.h"
# include "ServerError.h"

class UnixServerSocket : public IServerSocket<int>
{
public:
  UnixServerSocket();
  UnixServerSocket(int, sockaddr_in *);
  virtual ~UnixServerSocket();
  virtual int absReadFromClient(char *&, unsigned) const;
  virtual bool absWriteOnClient(char *, size_t) const;
  virtual std::string   getIP() const;
private:
  int sockfd;
  sockaddr_in *addr;
public:
	virtual inline int			getSocket() const { return(sockfd); }
protected:
	virtual inline void				setSocket(int socket) { this->sockfd = socket; }
};

#endif // !UNIXSERVERSOCKET_H_