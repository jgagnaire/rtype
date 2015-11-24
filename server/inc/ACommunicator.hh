#ifndef RTYPE_ICOMMUNICATOR_H
#define RTYPE_ICOMMUNICATOR_H

# include <list>
# include <stddef.h>
# include "IServerMonitor.hh"
# include "IServerMonitor.hh"
# include "ISocketSet.hh"

template <typename CONTROLLER, typename SCK>
class NetworkManager;

template <typename USER, typename CONTROLLER, typename MONITOR, typename SCK>
class ACommunicator {

protected:

  IServerMonitor<SCK>		*network_monitor;
  std::list<USER *>	    *cl_list;
  std::list<CONTROLLER *>	controllers;
  ISocketSet<SCK>		    *srvset;

  typename NetworkManager<SCK, CONTROLLER>::trigger			newConnection;
  typename NetworkManager<SCK, CONTROLLER>::trigger			closeConnection;
  typename NetworkManager<SCK, CONTROLLER>::Timeout			timeout;
  typename NetworkManager<SCK, CONTROLLER>::AfterObserve		aobs;
  typename NetworkManager<SCK, CONTROLLER>::triggerNew			newData;

  virtual inline void	timeoutAction() { ((controllers.front())->*timeout)(cl_list, this->network_monitor); }
  virtual inline void	afterObserveAction() { ((controllers.front())->*aobs)(cl_list, this->network_monitor); }
  virtual inline bool	acceptAction(USER *) { return (true); }
  virtual inline bool	connectAction(USER *) { return (true); }
  virtual inline bool	writeAction(USER *) { return (true); }
  virtual inline bool	readAction(USER *cli) { return (true); }
  virtual ~ACommunicator() {}

  virtual bool	closeAction(USER *cli) {
    if (closeConnection != NULL) {
      for (auto it = controllers.begin(); it != controllers.end(); ++it) {
	((*it)->*closeConnection)(cli);
      }
    }
    return (false);
  }

public:
  static void    _launch(std::vector<void *> arg) {
    NetworkManager<CONTROLLER, SCK>  *netmng = reinterpret_cast<NetworkManager<CONTROLLER, SCK> *>(arg[0]);
    ACommunicator   *ac = reinterpret_cast<ACommunicator *>(arg[1]);
    ac->launch(netmng->getClientList());
  }

  virtual inline void	addController(CONTROLLER *c) {
    controllers.push_back(c);
  }

  virtual inline void	triggerConnection(typename NetworkManager<SCK, CONTROLLER>::trigger connection) {
    newConnection = connection;
  }

  virtual inline void	triggerClose(typename NetworkManager<SCK, CONTROLLER>::trigger close) {
    closeConnection = close;
  }

  virtual inline void	triggerNewData(typename NetworkManager<SCK, CONTROLLER>::triggerNew data) {
    newData = data;
  }

  virtual inline void	triggerTimeout(typename NetworkManager<SCK, CONTROLLER>::Timeout t) {
    timeout = t;
  }

  virtual inline void	triggerAObserve(typename NetworkManager<SCK, CONTROLLER>::AfterObserve ao) {
    aobs = ao;
}

  virtual void	launch(std::list<USER*> *) = 0;

};

#endif
