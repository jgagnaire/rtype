#ifndef RTYPE_ICOMMUNICATOR_H
#define RTYPE_ICOMMUNICATOR_H

#include <list>
#include <stddef.h>
# include "IServerMonitor.hh"
# include "IServerMonitor.hh"
# include "ISocketSet.hh"

template <typename USER, typename CONTROLLER, typename MONITOR, typename SCK>
class ACommunicator {
protected:
    IServerMonitor<SCK>		*network_monitor;
    std::list<USER *>	    cl_list;
    std::list<CONTROLLER *>	controllers;

    trigger                 newConnection;
    trigger                 closeConnection;
    Timeout                 timeout;
    AfterObserve	        aobs;
    triggerNew              newData;


    typedef int	    (CONTROLLER::*trigger)(USER *) const;
    typedef int	    (CONTROLLER::*triggerNew)(USER *);
    typedef void	(CONTROLLER::*Timeout)(std::list<USER *> &, MONITOR *) const;
    typedef void	(CONTROLLER::*AfterObserve)(std::list<USER *> &, MONITOR *) const;

    virtual void    addController(CONTROLLER *c) { controllers.push_back(c); }
    virtual void	triggerConnection(trigger connection) { newConnection = connection; }
    virtual void	triggerClose(trigger close) { closeConnection = close; }
    virtual void	triggerNewData(triggerNew data) { newData = data; }
    virtual void	triggerTimeout(Timeout t) { timeout = t; }
    virtual void	triggerAObserve(AfterObserve ao) { aobs = ao; }
    virtual void	timeoutAction() {  ((controllers.front())->*timeout)(cl_list, this->network_monitor); }
    virtual void	afterObserveAction() { ((controllers.front())->*aobs)(cl_list, this->network_monitor); }
    virtual bool	acceptAction(USER *) { return (true); }
    virtual bool	connectAction(USER *) { return (true); }
    virtual bool	writeAction(USER *) { return (true); }
    virtual bool	readAction(USER *cli) { return (true); }

    virtual bool	closeAction(USER *cli) {
        if (closeConnection != NULL) {
            for (auto it = controllers.begin(); it != controllers.end(); ++it) {
                ((*it)->*closeConnection)(cli);
            }
        }
        return (false);
    }

public:
    virtual void	launch() = 0;

};

#endif
