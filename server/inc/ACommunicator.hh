#ifndef RTYPE_ICOMMUNICATOR_H
#define RTYPE_ICOMMUNICATOR_H

# include <list>
# include <stddef.h>
# include "IServerMonitor.hh"
# include "IServerMonitor.hh"
# include "ISocketSet.hh"
# include "UserManager.hh"

template <typename CONTROLLER, typename SCK>
class NetworkManager;

template <typename USER, typename CONTROLLER, typename MONITOR, typename SCK>
class ACommunicator {
public:
  typedef int   (CONTROLLER::*trigger)(UserManager<SCK> *) const;
  typedef int   (CONTROLLER::*triggerNew)(UserManager<SCK> *);
  typedef void  (CONTROLLER::*Timeout)(std::list<UserManager<SCK> *> &, IServerMonitor<SCK> *) const;
  typedef void  (CONTROLLER::*AfterObserve)(std::list<UserManager<SCK> *> &, IServerMonitor<SCK> *) const;
protected:

  IServerMonitor<SCK>		*network_monitor;
  std::list<USER *>		    *cl_list;
  std::list<CONTROLLER *>	controllers;
  ISocketSet<SCK>		    *srvset;

  trigger                   newConnection;
  trigger                   closeConnection;
  Timeout                   timeout;
  AfterObserve	            aobs;
  triggerNew                newData;

public:
    virtual ~ACommunicator() {}

    static void    _launch(std::vector<void *> arg) {
      NetworkManager<SCK, CONTROLLER>  *netmng = reinterpret_cast<NetworkManager<SCK, CONTROLLER> *>(arg[0]);
      ACommunicator   *ac = reinterpret_cast<ACommunicator *>(arg[1]);
      ac->launch(netmng->getClientList());
    }

    virtual inline void	addController(CONTROLLER *c) { controllers.push_back(c); }
    virtual inline void	triggerConnection(trigger connection) { newConnection = connection; }
    virtual inline void	triggerClose(trigger close) { closeConnection = close; }
    virtual inline void	triggerNewData(triggerNew data) { newData = data; }
    virtual inline void	triggerTimeout(Timeout t) { timeout = t; }
    virtual inline void	triggerAObserve(AfterObserve ao) { aobs = ao; }
    virtual void	    launch(std::list<USER*> *) = 0;

protected:

    virtual inline void	timeoutAction() { ((controllers.front())->*timeout)(*cl_list, this->network_monitor); }
    virtual inline void	afterObserveAction() { ((controllers.front())->*aobs)(*cl_list, this->network_monitor); }
    virtual inline bool	acceptAction(USER *) { return (true); }
    virtual inline bool	connectAction(USER *) { return (true); }
    virtual inline bool	writeAction(USER *) { return (true); }
    virtual inline bool	readAction(USER *) { return (true); }
    virtual bool	    closeAction(USER *cli) {
        if (closeConnection != NULL) {
            for (auto it = controllers.begin(); it != controllers.end(); ++it) {
                ((*it)->*closeConnection)(cli);
            }
        }
        return (false);
    }
};

#endif
