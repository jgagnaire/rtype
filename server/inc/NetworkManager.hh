#ifndef NETWORKMANAGER_H_
# define NETWORKMANAGER_H_

# include <cstdlib>
# include <list>
# include <vector>
# include "UserManager.h"
# include "Enum.h"

# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#  include "WinServerMonitor.h"
#  include "WinServerSocket.h"
#  include "WinSocketSet.h"
# else
#  include "UnixServerMonitor.h"
#  include "UnixServerSocket.h"
#  include "UnixSocketSet.h"
# endif

template <typename SCK, typename CONTROLLER>
class NetworkManager
{
  typedef int	(CONTROLLER::*trigger)(UserManager<SCK> *) const;
  typedef int	(CONTROLLER::*triggerNew)(UserManager<SCK> *);
  typedef void	(CONTROLLER::*Timeout)(std::list<UserManager<SCK> *> &, IServerMonitor<SCK> *) const;
  typedef void	(CONTROLLER::*AfterObserve)(std::list<UserManager<SCK> *> &, IServerMonitor<SCK> *) const;
  typedef bool	(NetworkManager::*monitor_ptr)(UserManager<SCK> *);

private:
  std::vector<monitor_ptr> monitor_action;
public:
  NetworkManager(char *port) : newConnection(NULL), closeConnection(NULL),
			       timeout(NULL), aobs(NULL) {
    monitor_action.push_back(&NetworkManager::readAction);
    monitor_action.push_back(&NetworkManager::writeAction);
    monitor_action.push_back(&NetworkManager::acceptAction);
    monitor_action.push_back(&NetworkManager::connectAction);
    monitor_action.push_back(&NetworkManager::closeAction);

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    this->network_monitor = new WinServerMonitor();
    this->srvset = new WinSocketSet(!port ? 6660 : std::atoi(port));
#else
    this->network_monitor = new UnixServerMonitor();
    this->srvset = new UnixSocketSet(!port ? 6660 : std::atoi(port));
#endif
    this->network_monitor->addFd(dynamic_cast<IServerSocket<SCK>*>(this->srvset),
				 static_cast<Enum::Flag>(Enum::ACCEPT));
  }
  ~NetworkManager() {}

  void	launch() {
    for (;;) {
      for (auto cli = cl_list.begin(); cli != cl_list.end();) {
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
	while (!(*cli)->sendStructEmpty()) {
	  if (!(*cli)->writeOnMe()) {
	    deleteClient(cli);
	    break;
	  }

	}
#else
	if (!(*cli)->sendStructEmpty()) {
	  this->network_monitor->setObserver((*cli)->getServerSocket(),
					     static_cast<Enum::Flag>(Enum::READ |
								     Enum::WRITE |
								     Enum::CLOSE));
	}
#endif
	++cli;
      }
      this->network_monitor->setTimeval(1, 0);
      if ((this->network_monitor->observerFds() == 0) &&
	  (timeout))
	timeoutAction();
      else
	checkObserver();
    }
  };

  void	deleteClient(typename std::list<UserManager<SCK> *>::iterator &cli) {
	this->network_monitor->closeFd((*cli)->getServerSocket());
    ((controllers.front())->*closeConnection)(*cli);
    delete *cli;
    *cli = NULL;
    cl_list.erase(cli);
    return;
  }

  void	checkObserver() {
    if (this->network_monitor->isObserved(dynamic_cast<IServerSocket<SCK>*>(this->srvset),
					  Enum::ACCEPT)) {
      IServerSocket<SCK>	*tmp = this->srvset->absAcceptNewClient();

      if (!tmp)
	return ;
      cl_list.push_back(new UserManager<SCK>(tmp));
      if (newConnection)
	for (auto it = controllers.begin(); it != controllers.end(); ++it)
	  ((*it)->*newConnection)(cl_list.back());
      this->network_monitor->addFd(dynamic_cast<IServerSocket<SCK>*>(cl_list.back()->getServerSocket()),
				   static_cast<Enum::Flag>(Enum::READ | Enum::WRITE | Enum::CLOSE));
      return ;
    }
    for (auto cli = cl_list.begin(); cli != cl_list.end(); ++cli) {
      for (int f = 0; (1 << f) <= Enum::CLOSE; ++f) {
	if (this->network_monitor->isObserved((*cli)->getServerSocket(),
					      static_cast<Enum::Flag>(1 << f))) {
	  if (!(this->*monitor_action[f])(*cli))
	    return (deleteClient(cli));
	}
      }
    }
    if (aobs)
      afterObserveAction();
  }

  std::list<UserManager<SCK> *>  &getClientList() { return (cl_list); }

  void	triggerConnection(trigger connection) { newConnection = connection; }
  void	triggerClose(trigger close) { closeConnection = close; }
  void	triggerNewData(triggerNew data) { newData = data; }
  void	triggerTimeout(Timeout t) { timeout = t; }
  void	triggerAObserve(AfterObserve ao) { aobs = ao; }
  void	operator<<(CONTROLLER *c) { controllers.push_back(c); }

private:
  bool		closeAction(UserManager<SCK> *cli) {
    if (closeConnection != NULL) {
      for (auto it = controllers.begin(); it != controllers.end(); ++it) {
	((*it)->*closeConnection)(cli);
      }
    }
    return (false); 
  }

  void		timeoutAction() {
    ((controllers.front())->*timeout)(cl_list, this->network_monitor);
  }

  void		afterObserveAction() {
    ((controllers.front())->*aobs)(cl_list, this->network_monitor);
  }

  bool		readAction(UserManager<SCK> *cli) {
    int		n;

    cli->readFromMe();
    if (cli->emptyData())
      return (false);
    if (!cli->IsFilled())
      return (true);
    for (auto it = controllers.begin(); it != controllers.end(); ++it) {
      if ((n = ((*it)->*newData)(cli)) == 1)
	{
	  cli->clearData();
	  this->network_monitor->setObserver(cli->getServerSocket(),
					     static_cast<Enum::Flag>(Enum::WRITE |
								     Enum::READ |
								     Enum::CLOSE));
	  return (true);
	}
      else if (n == 0)
	return (false);
    }
    cli->clearData();
    return (true);
  }

  inline bool		writeAction(UserManager<SCK> *cli) {
    if (cli->sendStructEmpty()) {
      this->network_monitor->setObserver(cli->getServerSocket(),
					 static_cast<Enum::Flag>(Enum::READ | Enum::CLOSE));
      return true;
    }
    if (!cli->writeOnMe())
      return (false); 
    if (cli->sendStructEmpty()) {
      this->network_monitor->setObserver(cli->getServerSocket(),
					 static_cast<Enum::Flag>(Enum::READ | Enum::CLOSE));
    }
    return (true);
  }

  inline bool	acceptAction(UserManager<SCK> *) { return (true); }
  inline bool	connectAction(UserManager<SCK> *) { return (true); }

  IServerMonitor<SCK>		*network_monitor;
  ISocketSet<SCK>		*srvset;
  std::list<UserManager<SCK> *>	cl_list;
  std::list<CONTROLLER *>	controllers;

  trigger newConnection;
  trigger closeConnection;
  Timeout timeout;
  AfterObserve	aobs;
  triggerNew newData;
};

#endif // !NETWORKMANAGER_H_
