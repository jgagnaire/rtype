#include <iostream>
#include "UnixServerMonitor.h"
#include "ServerError.h"

UnixServerMonitor::UnixServerMonitor() {
  fds.clear();
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  fd_nbr = 2;
  timeout = NULL;
}

UnixServerMonitor::~UnixServerMonitor() {
}

void         UnixServerMonitor::closeFd(IServerSocket<int> *sock) {
  int fd = sock->getSocket();

  for (std::list<select_info *>::iterator it = fds.begin();
       it != fds.end(); ++it) {
    if ((*it)->fd == fd)
      {
	if (fd != 0)
	  close(fd);
	delete *it;
	fds.erase(it);
	return;
      }
  }
}

void         UnixServerMonitor::deleteFd(IServerSocket<int> *sock) {
  int fd = sock->getSocket();

  for (std::list<select_info *>::iterator it = fds.begin();
       it != fds.end(); ++it) {
    if ((*it)->fd == fd)
      {
	delete *it;
	fds.erase(it);
	return;
      }
  }
}

int  UnixServerMonitor::observerFds() {
  int  ret;

  fd_nbr = 2;
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  for (std::list<select_info *>::iterator it = fds.begin();
       it != fds.end(); ++it) {
    setObserver((*it)->fd, (*it)->flag);
  }
  do {
    ret = select(fd_nbr + 1, &readfds, &writefds, NULL, timeout);
  }
  while (errno == EINTR && ret == -1);
  if (ret == -1)
    throw EMonitorFailed();
  return (ret);
}

void  UnixServerMonitor::setObserver(IServerSocket<int> *sock, Enum::Flag socket_flag) {
  int fd = sock->getSocket();

  setObserver(fd, socket_flag);
}

void  UnixServerMonitor::setObserver(int fd, Enum::Flag socket_flag) {
  for (std::list<select_info *>::iterator it = fds.begin();
       it != fds.end(); ++it) {
    if ((*it)->fd == fd)
      {
	for (int n = 0; (1 << n) <= static_cast<int>(Enum::CLOSE); ++n) {
	  if (static_cast<Enum::Flag>(1 << n) & socket_flag) {
	    if (static_cast<Enum::Flag>(1 << n) & Enum::WRITE)
	      FD_SET(fd, &writefds);
	    else
	      FD_SET(fd, &readfds);
	  }
	}
	if (fd_nbr < fd)
	  fd_nbr = fd;
	(*it)->flag = socket_flag;
	return ;
      }
  }
}

void  UnixServerMonitor::setTimeval(unsigned sec, unsigned microsec) {
  if (timeout)
    delete timeout;
  timeout = NULL;
  if (sec == static_cast<unsigned>(-1) || microsec == static_cast<unsigned>(-1))
    return;
  timeout = new struct timeval;
  if (!timeout)
    throw; //TODO ! Exception
  timeout->tv_sec = sec;
  timeout->tv_usec = microsec;
}

void  UnixServerMonitor::deleteTimeval() {
  if (timeout)
    delete timeout;
  timeout = NULL;
}

void  UnixServerMonitor::addFd(IServerSocket<int> *sock, Enum::Flag socket_flag = Enum::NONE) {
  int fd = sock->getSocket();
  select_info *s = new select_info;

  if (!s)
    throw; //TODO ! Exception
  s->fd = fd;
  s->flag = socket_flag;
  fds.push_back(s);
  setObserver(fd, socket_flag);
}

bool  UnixServerMonitor::isObserved(IServerSocket<int> *sock, Enum::Flag socket_flag) {
  select_info *s;
  int fd = sock->getSocket();

  if (socket_flag > Enum::WRITE && fd != fds.front()->fd)
    return (false);
  for (std::list<select_info *>::iterator it = fds.begin(); it != fds.end(); ++it)
    if ((*it)->fd == fd)
      {
	s = *it;
	break ;
      }
  if (s->flag & socket_flag) {
    if (socket_flag == Enum::WRITE)
      return (!!FD_ISSET(fd, &writefds));
    return (!!FD_ISSET(fd, &readfds));
  }
  return (false);
}
