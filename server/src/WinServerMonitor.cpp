#include "WinServerMonitor.hh"
#include "ServerError.hh"

WinServerMonitor::WinServerMonitor()
	:_max_fd(0), _timeout(WSA_INFINITE)
{
	_loop_event = 0;
	::ZeroMemory(_events, WSA_MAXIMUM_WAIT_EVENTS * sizeof(WSAEVENT));
	::ZeroMemory(_sockets, WSA_MAXIMUM_WAIT_EVENTS * sizeof(SOCKET));
	flags.push_back({ Enum::Flag::READ, FD_READ });
	flags.push_back({ Enum::Flag::WRITE, FD_WRITE });
	flags.push_back({ Enum::Flag::ACCEPT, FD_ACCEPT });
	flags.push_back({ Enum::Flag::CONNECT, FD_CONNECT });
	flags.push_back({ Enum::Flag::CLOSE, FD_CLOSE });
}

WinServerMonitor::~WinServerMonitor() {
  for (int i = 0; i < static_cast<int>(WSA_MAXIMUM_WAIT_EVENTS); ++i) {
    if (_sockets[i] != 0) {
      ::closesocket(static_cast<SOCKET>(_sockets[i]));
      ::WSACloseEvent(_events[i]);
    }
    _sockets[i] = 0;
    _events[i] = 0;
  }
}

void	WinServerMonitor::addFd(IServerSocket<SOCKET> *sock, Enum::Flag socket_flag) {
  if (!sock)
    throw ESockDynaFail();
  SOCKET fd = sock->getSocket();
  int	i;
  long	win_flag = 0;

  for (i = 0; i < WSA_MAXIMUM_WAIT_EVENTS &&
	  _sockets[i] != 0; ++i);
  if (i == WSA_MAXIMUM_WAIT_EVENTS)
	return;
  _sockets[i] = fd;
  for (int n = 0; (1 << n) <= static_cast<int>(Enum::Flag::CLOSE); ++n) {
    if (socket_flag & flags[n].monitor_flag)
      win_flag |= flags[n].win_flag;
  }
  if ((_events[i] = WSACreateEvent()) == WSA_INVALID_EVENT)
    throw ESockCreateEvent();
  if (::WSAEventSelect(_sockets[i], _events[i], win_flag)
      == SOCKET_ERROR)
    throw ESockEventSelect();
  ++_max_fd;
}

void   WinServerMonitor::closeFd(IServerSocket<SOCKET> *sock) {
  if (!sock)
    throw ESockDynaFail();
  SOCKET fd = sock->getSocket();
  for (int i = 0; i < static_cast<int>(WSA_MAXIMUM_WAIT_EVENTS); ++i) {
    if (_sockets[i] == fd && fd != 0) {
      ::closesocket(fd);
      ::WSACloseEvent(_events[i]);
      _sockets[i] = 0;
      _events[i] = 0;
	  for (i; i < WSA_MAXIMUM_WAIT_EVENTS - 1; ++i) {
		  _sockets[i] = _sockets[i + 1];
		  _events[i] = _events[i + 1];
	  }
      --_max_fd;
      break;
    }
  }
}

void   WinServerMonitor::deleteFd(IServerSocket<SOCKET> *sock) {
  if (!sock)
    throw ESockDynaFail();
  SOCKET fd = sock->getSocket();
  for (int i = 0; i < static_cast<int>(WSA_MAXIMUM_WAIT_EVENTS); ++i) {
    if (_sockets[i] == fd && fd != 0) {
      ::WSACloseEvent(_events[i]);
      _sockets[i] = 0;
      _events[i] = 0;
	  for (i; i < WSA_MAXIMUM_WAIT_EVENTS - 1; ++i) {
		  _sockets[i] = _sockets[i + 1];
		  _events[i] = _events[i + 1];
	  }
      --_max_fd;
      break;
    }
  }
}

int	   WinServerMonitor::observerFds() {
  _ret_val = ::WSAWaitForMultipleEvents(_max_fd, _events, FALSE, _timeout, FALSE);
  if (_ret_val == WSA_WAIT_FAILED)
	  return (-1);
  if (WSA_WAIT_TIMEOUT == _ret_val)
    return (0);
  _loop_event = 0;
  return (1);
}

void   WinServerMonitor::setObserver(IServerSocket<SOCKET> *sock, Enum::Flag socket_flag) {
  if (!sock)
    throw ESockDynaFail();
  SOCKET fd = sock->getSocket();
  long win_flag = 0;

  for (int i = 0; i < static_cast<int>(WSA_MAXIMUM_WAIT_EVENTS); ++i) {
    if (_sockets[i] == fd) {
	  for (int n = 0; (1 << n) <= static_cast<int>(Enum::Flag::CLOSE); ++n) {
		  if (socket_flag & static_cast<Enum::Flag>(1 << n)) {
			  win_flag |= flags[n].win_flag;
		  }
      }
	  if (::WSAEventSelect(_sockets[i], _events[i], win_flag)
		  == SOCKET_ERROR)
		  throw ESockEventSelect();
	  return;
    }
  }
}

bool   WinServerMonitor::isObserved(IServerSocket<SOCKET> *sock, Enum::Flag socket_flag) {
  if (!sock)
    throw ESockDynaFail();
  SOCKET fd = sock->getSocket();
  static int			  tmp_fd;
  static WSANETWORKEVENTS net_events;

  if (_sockets[0] == fd) {
    tmp_fd = fd;
    _loop_event = 0;
    ::WSAWaitForMultipleEvents(1, &_events[_loop_event], FALSE, 0, FALSE);
    ::WSAEnumNetworkEvents(_sockets[_loop_event], _events[_loop_event], &net_events);
  }
  else if (tmp_fd != fd) {
    tmp_fd = fd;
    ++_loop_event;
    while ((_loop_event != WSA_MAXIMUM_WAIT_EVENTS) && !_sockets[_loop_event])
      ++_loop_event;
    if (_loop_event == WSA_MAXIMUM_WAIT_EVENTS)
      return (false);
    ::WSAWaitForMultipleEvents(1, &_events[_loop_event], FALSE, 0, FALSE);
    ::WSAEnumNetworkEvents(_sockets[_loop_event], _events[_loop_event], &net_events);
  }
  for (int n = 0; (1 << n) <= static_cast<int>(Enum::Flag::CLOSE); ++n) {
    if (socket_flag & flags[n].monitor_flag) {
      return (!!(net_events.lNetworkEvents & flags[n].win_flag));
    }
  }
  return (false);
}

inline
void   WinServerMonitor::setTimeval(unsigned sec, unsigned milli) {
  _timeout = sec * 1000 + milli;
}

inline
void   WinServerMonitor::deleteTimeval() {
  _timeout = WSA_INFINITE;
}
