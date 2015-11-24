#include <iostream>
#include "AccountController.hh"
#include "NetworkManager.hh"
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
	  NetworkManager<int, AController<int> > netmgr(av[1]);
	  netmgr.triggerConnection(&AController<int>::newConnection);
	  netmgr.triggerNewData(&AController<int>::newData);
	  netmgr.triggerClose(&AController<int>::closeConnection);
	  netmgr.triggerTimeout(&AController<int>::timeout);
	  netmgr.triggerAObserve(&AController<int>::timeout);
	  netmgr.addController(new AccountController<int>(netmgr.getClientList()));
#endif
	netmgr.launch();
  }
  
  catch (ESockCreate const &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  catch (ESockAccept const &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  catch (ESockRead const &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  catch (ESockWrite const &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  catch (ServerError const &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  catch (std::bad_alloc const &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  return (0);
}
