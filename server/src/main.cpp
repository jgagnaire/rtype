#include <iostream>
#include "JSONParser.hh"
#include "AccountController.hh"
#include "NetworkManager.hh"
#include "GameController.hh"
#include "GameplayController.hh"
#include "Enum.hh"
#include "ServerError.hh"

#if !(defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64))
# include <sys/stat.h>
#endif

int main(int, char **av)
{
  try {
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
	CreateDirectory("./server/.database", NULL);
	NetworkManager<SOCKET, AController<SOCKET> > netmgr(av[1]);
	netmgr.triggerConnection(&AController<SOCKET>::newConnection);
	netmgr.triggerNewData(&AController<SOCKET>::newData);
	netmgr.triggerClose(&AController<SOCKET>::closeConnection);
	netmgr.triggerTimeout(&AController<SOCKET>::timeout);
	netmgr.triggerAObserve(&AController<SOCKET>::timeout);
	netmgr.addController(new AccountController<SOCKET>(netmgr.getClientList()));
#else
	  mkdir("./server/.database", 0755);
	  NetworkManager<int, AController<int> > netmgr(av[1], Enum::BOTH_PROTO);
	  netmgr.triggerConnection(&AController<int>::newConnection);
	  netmgr.triggerNewData(&AController<int>::newData);
	  netmgr.triggerClose(&AController<int>::closeConnection);
	  netmgr.triggerTimeout(&AController<int>::timeout);
	  netmgr.triggerAObserve(&AController<int>::timeout);
      netmgr.addController(new AccountController<int>(netmgr.getClientList()));
      netmgr.addController(new GameController<int>(netmgr.getClientList()));
      netmgr.addController(new GameplayController<int>(netmgr.getClientList()), Enum::UDP);
#endif
	netmgr.launch();
  }

  catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  return (0);
}
