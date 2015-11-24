#ifndef NETWORKMANAGER_H_
# define NETWORKMANAGER_H_

# include <cstdlib>
# include <list>
# include <vector>
# include "UserManager.hh"
# include "Enum.hh"

# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#  include "WinServerMonitor.hh"
#  include "WinServerSocket.hh"
#  include "WinTCPSocketSet.hh"
#  include "WinUDPSocketSet.hh"
# else
#  include "UnixServerMonitor.hh"
#  include "UnixServerSocket.hh"
#  include "UnixTCPSocketSet.hh"
#  include "UnixUDPSocketSet.hh"
# endif

template <typename SCK, typename CONTROLLER>
class NetworkManager
{
private:
  std::vector<monitor_ptr> monitor_action;
public:
  NetworkManager(char *port, Enum::Protocol p = Enum::TCP) {

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    this->network_monitor = new WinServerMonitor();
    if (static_cast<int>(Enum::Enum::TCP) & static_cast<int>(p))
      this->srvset[Enum::TCP_INDEX] = new WinTCPSocketSet(!port ? 1119 : std::atoi(port));
    if (static_cast<int>(Enum::Enum::UDP}) & static_cast<int>(p))
      this->srvset[Enum::UDP_INDEX] = new WinUDPSocketSet(!port ? 1725 : std::atoi(port));
#else
    this->network_monitor = new UnixServerMonitor();
    if (static_cast<int>(Enum::Enum::TCP) & static_cast<int>(p))
      this->srvset[Enum::TCP_INDEX] = new UnixTCPSocketSet(!port ? 1119 : std::atoi(port));
    if (static_cast<int>(Enum::Enum::UDP) & static_cast<int>(p))
      this->srvset[Enum::UDP_INDEX] = new UnixUDPSocketSet(!port ? 1725 : std::atoi(port));
#endif
    this->network_monitor->addFd(dynamic_cast<IServerSocket<SCK>*>(this->srvset[Enum::TCP_INDEX]),
				 static_cast<Enum::Flag>(Enum::ACCEPT));
  }
  ~NetworkManager() {}
};

#endif // !NETWORKMANAGER_H_
